#!/usr/bin/env bash
# ==================================================================
# run_harness_batch_text.sh — Batch runner that logs human-readable
# results to a single text file (Bash 3 compatible, macOS-safe).
#
# Works with ./scripts/run_harness.sh that supports:
#   --mode=good|bad|both, --quiet, --emit=csv, --timeout=SECS
#
# Usage:
#   ./scripts/run_harness_batch_text.sh <CWE_NUMBER> \
#     [--mode=both|good|bad] [--out=PATH] [--append] [--timeout=SECS]
#
# Example:
#   ./scripts/run_harness_batch_text.sh 364 --out=build/cwe364_results.txt
#   ./scripts/run_harness_batch_text.sh 364 --mode=bad --out=build/cwe364_bad_only.txt
#
# Output format (one line per run):
#   [2025-11-11 14:37:22] CWE=364 | harness=CWE364_..._harness.c | mode=good | status=PASS | asan=no | exit=0 | log=build/...
#
# Summary is appended to the end of the file.
# ==================================================================
set -euo pipefail

if [ $# -lt 1 ]; then
  echo "Usage: $0 <CWE_NUMBER> [--mode=both|good|bad] [--out=PATH] [--append] [--timeout=SECS]"; exit 1
fi

CWE="$1"; shift || true
MODE="both"
OUT_PATH="harness_results/segrep/cwe${CWE}_results.txt"
APPEND=0
TIMEOUT_SECS="15"

while [ $# -gt 0 ]; do
  case "$1" in
    --mode=good) MODE="good";;
    --mode=bad)  MODE="bad";;
    --mode=both) MODE="both";;
    --out=*) OUT_PATH="${1#--out=}";;
    --out) shift || true; OUT_PATH="${1:-}";;
    --append) APPEND=1;;
    --timeout=*) TIMEOUT_SECS="${1#--timeout=}";;
    --timeout) shift || true; TIMEOUT_SECS="${1:-15}";;
    *) echo "Unknown option: $1"; exit 1;;
  esac
  shift || true
done

RUN="./scripts/run_harness.sh"
GEN_DIR="generated_test_cases/CWE${CWE}"
[ -x "$RUN" ] || { echo "❌ Not executable or missing: $RUN"; exit 1; }
[ -d "$GEN_DIR" ] || { echo "❌ No harness dir: $GEN_DIR"; exit 1; }

mkdir -p "$(dirname "$OUT_PATH")" || true
if [ $APPEND -eq 0 ]; then
  : > "$OUT_PATH"
  printf "# Results for CWE%s — %s\n" "$CWE" "$(date '+%Y-%m-%d %H:%M:%S')" >> "$OUT_PATH"
fi

HARNESS_LIST_FILE="$(mktemp)"
find "$GEN_DIR" -type f -name '*_harness.c' 2>/dev/null | LC_ALL=C sort > "$HARNESS_LIST_FILE"
TOTAL=$(wc -l < "$HARNESS_LIST_FILE" | tr -d ' ')
[ "$TOTAL" -gt 0 ] || { echo "❌ No *_harness.c files under $GEN_DIR"; rm -f "$HARNESS_LIST_FILE"; exit 1; }

echo "🧪 Found $TOTAL harness(es) under $GEN_DIR"
good_total=0; good_pass=0
bad_total=0;  bad_pass=0

log_line() {
  # $1=harness $2=mode $3=status(PASS/FAIL) $4=asan(yes/no) $5=exit $6=logpath
  local ts; ts="$(date '+%Y-%m-%d %H:%M:%S')"
  printf "[%s] CWE=%s | harness=%s | mode=%s | status=%s | asan=%s | exit=%s | log=%s\n" \
    "$ts" "$CWE" "$1" "$2" "$3" "$4" "$5" "$6" >> "$OUT_PATH"
}

while IFS= read -r full; do
  [ -z "$full" ] && continue
  h="$(basename "$full")"
  echo "• $h"

  if [ "$MODE" = "both" ] || [ "$MODE" = "good" ]; then
    good_total=$((good_total+1))
    CSV_LINE="$("$RUN" "$CWE" "$h" --mode=good --quiet --emit=csv --timeout="$TIMEOUT_SECS" || true)"
    IFS=',' read -r _cwe _h _mode _status _asan _exit _log <<< "$CSV_LINE"
    if [ "${_status:-FAIL}" = "PASS" ]; then
      good_pass=$((good_pass+1)); echo "   GOOD: PASS"
    else
      echo "   GOOD: FAIL"
    fi
    log_line "$h" "good" "${_status:-FAIL}" "${_asan:-no}" "${_exit:-1}" "${_log:-}"
  fi

  if [ "$MODE" = "both" ] || [ "$MODE" = "bad" ]; then
    bad_total=$((bad_total+1))
    CSV_LINE="$("$RUN" "$CWE" "$h" --mode=bad --quiet --emit=csv --timeout="$TIMEOUT_SECS" || true)"
    IFS=',' read -r _cwe _h _mode _status _asan _exit _log <<< "$CSV_LINE"
    if [ "${_status:-FAIL}" = "PASS" ]; then
      bad_pass=$((bad_pass+1)); echo "   BAD:  PASS"
    else
      echo "   BAD:  FAIL"
    fi
    log_line "$h" "bad" "${_status:-FAIL}" "${_asan:-no}" "${_exit:-1}" "${_log:-}"
  fi
done < "$HARNESS_LIST_FILE"

rm -f "$HARNESS_LIST_FILE"

percent() { local n="$1" d="$2"; if [ "$d" -eq 0 ]; then printf "0.0"; else awk "BEGIN{printf \"%.1f\", ($n*100.0)/$d}"; fi; }

{
  echo
  echo "================ Summary ================"
  if [ "$MODE" = "good" ] || [ "$MODE" = "both" ]; then
    printf "GOOD: %d / %d  (%s%%)  (clean runs expected)\n" "$good_pass" "$good_total" "$(percent "$good_pass" "$good_total")"
  fi
  if [ "$MODE" = "bad" ] || [ "$MODE" = "both" ]; then
    printf "BAD:  %d / %d  (%s%%)   (ASan expected)\n" "$bad_pass" "$bad_total" "$(percent "$bad_pass" "$bad_total")"
  fi
  echo "========================================="
} >> "$OUT_PATH"

echo "📄 Wrote: $OUT_PATH"
