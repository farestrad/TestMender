#!/usr/bin/env bash
set -euo pipefail

if [ $# -lt 1 ]; then
  echo "Usage: $0 <CWE_NUMBER> [--tool=cppcheck|semgrep] [--mode=both|good|bad] [--out=PATH] [--append] [--timeout=SECS]"
  exit 1
fi

CWE="$1"; shift || true

# -----------------------------
# DEFAULTS
# -----------------------------
TOOL="cppcheck"     # or semgrep
MODE="both"
OUT_PATH=""         # will be set after TOOL is known

APPEND=0
TIMEOUT_SECS="15"

while [ $# -gt 0 ]; do
  case "$1" in
    --tool=cppcheck) TOOL="cppcheck";;
    --tool=semgrep)  TOOL="semgrep";;
    --mode=good)     MODE="good";;
    --mode=bad)      MODE="bad";;
    --mode=both)     MODE="both";;
    --out=*)         OUT_PATH="${1#--out=}";;
    --out)           shift || true; OUT_PATH="${1:-}";;
    --append)        APPEND=1;;
    --timeout=*)     TIMEOUT_SECS="${1#--timeout=}";;
    --timeout)       shift || true; TIMEOUT_SECS="${1:-15}";;
    *) echo "Unknown option: $1"; exit 1;;
  esac
  shift || true
done

# If OUT_PATH wasn't explicitly set, use a default that depends on TOOL
if [ -z "${OUT_PATH:-}" ]; then
  OUT_PATH="harness_results/improved_${TOOL}/cwe${CWE}_results.txt"
fi

RUN="./scripts/run_harness.sh"

# Directory naming:
#   Python wrote: improved_tests/<tool>/cwe${CWE}/...
#   e.g., improved_tests/cppcheck/cwe121/...
CWE_DIR_NUM="CWE${CWE}"
CWE_DIR_LOWER="cwe${CWE}"

GEN_DIR="improved_tests/${TOOL}/${CWE_DIR_LOWER}"
HARNESS_BASE_DIR="improved_tests/${TOOL}"

[ -x "$RUN" ] || { echo "❌ Not executable or missing: $RUN"; exit 1; }
[ -d "$GEN_DIR" ] || { echo "❌ No harness dir: $GEN_DIR"; exit 1; }

mkdir -p "$(dirname "$OUT_PATH")" || true
if [ $APPEND -eq 0 ]; then
  : > "$OUT_PATH"
  printf "# Improved results for CWE%s (%s) — %s\n" \
    "$CWE" "$TOOL" "$(date '+%Y-%m-%d %H:%M:%S')" >> "$OUT_PATH"
fi

HARNESS_LIST_FILE="$(mktemp)"
# New harnesses look like: *_harness_improved.c
find "$GEN_DIR" -type f -name '*_harness_improved.c' 2>/dev/null | LC_ALL=C sort > "$HARNESS_LIST_FILE"
TOTAL=$(wc -l < "$HARNESS_LIST_FILE" | tr -d ' ')
[ "$TOTAL" -gt 0 ] || { echo "❌ No *_harness_improved.c files under $GEN_DIR"; rm -f "$HARNESS_LIST_FILE"; exit 1; }

echo "🧪 Found $TOTAL improved harness(es) under $GEN_DIR"
good_total=0; good_pass=0
bad_total=0;  bad_pass=0

# JSONL output for validator results (separate folder for improved tests)
JSON_OUT="analysis_results/validator_json_${TOOL}_improved/cwe${CWE}_results.jsonl"
mkdir -p "$(dirname "$JSON_OUT")"
: > "$JSON_OUT"

log_line() {
  # $1=harness $2=mode $3=status(PASS/FAIL) $4=asan(yes/no) $5=exit $6=logpath
  local ts; ts="$(date '+%Y-%m-%d %H:%M:%S')"
  printf "[%s] CWE=%s | tool=%s | harness=%s | mode=%s | status=%s | asan=%s | exit=%s | log=%s\n" \
    "$ts" "$CWE" "$TOOL" "$1" "$2" "$3" "$4" "$5" "$6" >> "$OUT_PATH"
}

while IFS= read -r full; do
  [ -z "$full" ] && continue
  h="$(basename "$full")"
  echo "• $h"

  # per-harness tracking for validator/json
  good_status="NA"; good_exit=""
  bad_status="NA";  bad_exit=""

  if [ "$MODE" = "both" ] || [ "$MODE" = "good" ]; then
    good_total=$((good_total+1))
    CSV_LINE="$(HARNESS_BASE_DIR="$HARNESS_BASE_DIR" "$RUN" "$CWE" "$h" --mode=good --quiet --emit=csv --timeout="$TIMEOUT_SECS" || true)"

    IFS=',' read -r _cwe _h _mode _status _asan _exit _log <<< "$CSV_LINE"
    if [ "${_status:-FAIL}" = "PASS" ]; then
      good_pass=$((good_pass+1)); echo "   GOOD: PASS"
      good_status="PASS"; good_exit="${_exit:-0}"
    else
      echo "   GOOD: FAIL"
      good_status="FAIL"; good_exit="${_exit:-1}"
    fi
    log_line "$h" "good" "${_status:-FAIL}" "${_asan:-no}" "${_exit:-1}" "${_log:-}"
  fi

  if [ "$MODE" = "both" ] || [ "$MODE" = "bad" ]; then
    bad_total=$((bad_total+1))
    CSV_LINE="$(HARNESS_BASE_DIR="$HARNESS_BASE_DIR" "$RUN" "$CWE" "$h" --mode=bad --quiet --emit=csv --timeout="$TIMEOUT_SECS" || true)"

    IFS=',' read -r _cwe _h _mode _status _asan _exit _log <<< "$CSV_LINE"
    if [ "${_status:-FAIL}" = "PASS" ]; then
      bad_pass=$((bad_pass+1)); echo "   BAD:  PASS"
      bad_status="PASS"; bad_exit="${_exit:-0}"
    else
      echo "   BAD:  FAIL"
      bad_status="FAIL"; bad_exit="${_exit:-1}"
    fi
    log_line "$h" "bad" "${_status:-FAIL}" "${_asan:-no}" "${_exit:-1}" "${_log:-}"
  fi

  # --- Validator decision + JSON + Final Suite ---
  validated="false"
  reason=""

  if [ "$MODE" = "both" ]; then
    if [ "$good_status" = "PASS" ] && [ "$bad_status" = "FAIL" ]; then
      validated="true"
      reason="validated"
    else
      if [ "$good_status" != "PASS" ] && [ "$good_status" != "NA" ]; then
        reason="good_failed"
      elif [ "$bad_status" != "FAIL" ] && [ "$bad_status" != "NA" ]; then
        reason="bad_passed"
      else
        reason="incomplete_runs"
      fi
    fi
  else
    reason="mode_${MODE}_only"
  fi

  # write JSONL entry
  printf '{"harness":"%s","tool":"%s","mode":"%s","good_status":"%s","good_exit":"%s","bad_status":"%s","bad_exit":"%s","validated":%s,"reason":"%s"}\n' \
    "$h" "$TOOL" "$MODE" "$good_status" "$good_exit" "$bad_status" "$bad_exit" "$validated" "$reason" >> "$JSON_OUT"

  # if validated, copy to final *improved* test suite
  final_dir="final_test_suite_${TOOL}_improved/${CWE_DIR_NUM}"
  if [ "$validated" = "true" ]; then
    mkdir -p "$final_dir"
    cp "$full" "$final_dir/"
  fi
done < "$HARNESS_LIST_FILE"

rm -f "$HARNESS_LIST_FILE"

percent() {
  local n="$1" d="$2"
  if [ "$d" -eq 0 ]; then
    printf "0.0"
  else
    awk "BEGIN{printf \"%.1f\", ($n*100.0)/$d}"
  fi
}

{
  echo
  echo "================ Summary (improved tests) ================"
  printf "TOOL: %s\n" "$TOOL"
  if [ "$MODE" = "good" ] || [ "$MODE" = "both" ]; then
    printf "GOOD: %d / %d  (%s%%)  (clean runs expected)\n" \
      "$good_pass" "$good_total" "$(percent "$good_pass" "$good_total")"
  fi
  if [ "$MODE" = "bad" ] || [ "$MODE" = "both" ]; then
    printf "BAD:  %d / %d  (%s%%)   (ASan expected)\n" \
      "$bad_pass" "$bad_total" "$(percent "$bad_pass" "$bad_total")"
  fi
  echo "========================================================="
} >> "$OUT_PATH"

echo "📄 Wrote: $OUT_PATH"
echo "💾 Validator JSONL (improved): $JSON_OUT"
echo "📁 Final suite (validated improved harnesses): final_test_suite_${TOOL}_improved/${CWE_DIR_NUM}/"
