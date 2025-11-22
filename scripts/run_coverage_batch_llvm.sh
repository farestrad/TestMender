#!/usr/bin/env bash
# ==================================================================
# run_coverage_batch_llvm.sh — Batch line coverage for GPT harnesses
# using clang + llvm-cov, with a per-binary timeout.
#
# Usage:
#   ./scripts/run_coverage_batch_llvm.sh <CWE_NUMBER>
#
# Output:
#   - coverage_results/cwe${CWE}_coverage_llvm.txt  (one line per harness)
#   - temporary per-harness profraw/profdata files under coverage_results/CWE${CWE}/
# ==================================================================
set -euo pipefail

if [ $# -lt 1 ]; then
  echo "Usage: $0 <CWE_NUMBER>" >&2
  exit 1
fi

CWE="$1"

HARNESS_DIR="generated_test_cases/CWE${CWE}"
JULIET_ROOT="data/juliet/testcases"
INCLUDE_JULIET="data/juliet/testcasesupport"
SUPPORT_SOURCES=(data/juliet/testcasesupport/*.c)

OUT_DIR="coverage_results/gpt/CWE${CWE}"
SUMMARY_OUT="coverage_results/gpt/cwe${CWE}_coverage_llvm.txt"
TIMEOUT_SECS=10

mkdir -p "$OUT_DIR"
mkdir -p "$(dirname "$SUMMARY_OUT")"

if [ ! -d "$HARNESS_DIR" ]; then
  echo "❌ No harness dir: $HARNESS_DIR" >&2
  exit 1
fi

# --------------------------------------------------------------
# Small timeout helper (works even if `timeout`/`gtimeout` missing)
# --------------------------------------------------------------
run_with_timeout() {
  local secs="$1"
  shift
  if command -v gtimeout >/dev/null 2>&1; then
    gtimeout "$secs" "$@"
  elif command -v timeout >/dev/null 2>&1; then
    timeout "$secs" "$@"
  else
    perl -e 'alarm shift @ARGV; exec @ARGV' "$secs" "$@"
  fi
}

# --------------------------------------------------------------
# Simple logger
# --------------------------------------------------------------
log_line() {
  # $1 = harness
  # $2 = status (OK|TIMEOUT|COMPILE_FAIL|COV_FAIL)
  # $3 = line_coverage (e.g., "81.3%" or "NA")
  local ts
  ts="$(date '+%Y-%m-%d %H:%M:%S')"
  printf "[%s] CWE=%s | harness=%s | status=%s | line_coverage=%s\n" \
    "$ts" "$CWE" "$1" "$2" "$3" >> "$SUMMARY_OUT"
}

# --------------------------------------------------------------
# Init summary file
# --------------------------------------------------------------
: > "$SUMMARY_OUT"
printf "# Coverage (llvm-cov) for CWE%s — %s\n" "$CWE" "$(date '+%Y-%m-%d %H:%M:%S')" >> "$SUMMARY_OUT"

echo "🧪 [COVERAGE] Searching harnesses under $HARNESS_DIR"

HARNESS_LIST=("$HARNESS_DIR"/*_harness.c)
if [ ${#HARNESS_LIST[@]} -eq 0 ]; then
  echo "❌ No *_harness.c files under $HARNESS_DIR" >&2
  exit 1
fi

idx=0
ok_count=0
timeout_count=0
compile_fail_count=0
cov_fail_count=0

# NEW: accumulators for average coverage
total_cov_sum="0.0"
total_cov_count=0

for HARNESS in "${HARNESS_LIST[@]}"; do
  idx=$((idx+1))
  HARNESS_BASE="$(basename "$HARNESS")"
  STEM="${HARNESS_BASE%_harness.c}"

  echo "👉 [$idx] Harness: $HARNESS_BASE"

  JULIET_SOURCE="$(find "$JULIET_ROOT" -name "${STEM}.c" | head -n 1 || true)"
  if [ -z "$JULIET_SOURCE" ]; then
    echo "   ❌ Could not find Juliet source for ${STEM}.c"
    log_line "$HARNESS_BASE" "COMPILE_FAIL" "NA"
    compile_fail_count=$((compile_fail_count+1))
    continue
  fi

  echo "   ✔ Juliet source: $JULIET_SOURCE"

  CLEAN_HARNESS="$OUT_DIR/${STEM}_clean.c"
  BIN="$OUT_DIR/${STEM}_cov"
  PROFRAW="$BIN.profraw"
  PROFDATA="$BIN.profdata"
  REPORT_TXT="$OUT_DIR/${STEM}_llvm_cov.txt"

  echo "   🧼 Cleaning harness (non-destructive)..."
  sed 's/^```c$//; s/^```$//' "$HARNESS" > "$CLEAN_HARNESS"

  echo "   ⛏️  Compiling with coverage (clang/llvm)..."
  if ! clang -I "$INCLUDE_JULIET" \
            -fprofile-instr-generate -fcoverage-mapping \
            "$CLEAN_HARNESS" "$JULIET_SOURCE" \
            "${SUPPORT_SOURCES[@]}" \
            -o "$BIN" >/dev/null 2>&1; then
    echo "   ❌ Compilation failed"
    log_line "$HARNESS_BASE" "COMPILE_FAIL" "NA"
    compile_fail_count=$((compile_fail_count+1))
    continue
  fi

  echo "   ▶️  Running test binary (timeout=${TIMEOUT_SECS}s)..."
  rm -f "$PROFRAW" "$PROFDATA" "$REPORT_TXT"
  if ! LLVM_PROFILE_FILE="$PROFRAW" run_with_timeout "$TIMEOUT_SECS" "$BIN" >/dev/null 2>&1; then
    echo "   ⚠️  Binary timed out or crashed"
    log_status="TIMEOUT"
    timeout_count=$((timeout_count+1))
  else
    log_status="OK"
    ok_count=$((ok_count+1))
  fi

  echo "   📊  Running llvm-cov..."
  line_cov="NA"

  if [ -f "$PROFRAW" ]; then
    if llvm-profdata merge -sparse "$PROFRAW" -o "$PROFDATA" >/dev/null 2>&1; then
      if llvm-cov report "$BIN" \
           -instr-profile="$PROFDATA" \
           "$JULIET_SOURCE" > "$REPORT_TXT" 2>/dev/null; then
        total_line="$(grep -E '^TOTAL' "$REPORT_TXT" || true)"
        if [ -n "$total_line" ]; then
          # last column is typically like "33.33%" or "-"
          line_cov="$(echo "$total_line" | awk '{print $NF}')"
        fi
      fi
    fi
  fi

  if [ "$line_cov" = "NA" ] || [ "$line_cov" = "-" ]; then
    echo "   ❌ Could not parse coverage from llvm-cov"
    if [ "$log_status" = "OK" ]; then
      log_status="COV_FAIL"
      cov_fail_count=$((cov_fail_count+1))
    fi
  else
    echo "   ✅ Line coverage: $line_cov"
    # --- accumulate numeric coverage for average ---
    cov_val="$(echo "$line_cov" | tr -d '%')"   # drop the %
    # use awk for floating-point addition
    total_cov_sum="$(awk -v a="$total_cov_sum" -v b="$cov_val" 'BEGIN{printf "%.6f", a+b}')"
    total_cov_count=$((total_cov_count+1))
  fi

  log_line "$HARNESS_BASE" "$log_status" "$line_cov"
done

# --------------------------------------------------------------
# Summary footer + average coverage
# --------------------------------------------------------------
{
  echo
  echo "================ Coverage Summary (llvm-cov) ================"
  printf "OK (ran, coverage parsed): %d\n" "$ok_count"
  printf "TIMEOUT / crash:          %d\n" "$timeout_count"
  printf "COMPILE_FAIL:             %d\n" "$compile_fail_count"
  printf "COV_FAIL (no report):     %d\n" "$cov_fail_count"

  if [ "$total_cov_count" -gt 0 ]; then
    avg_cov="$(awk -v s="$total_cov_sum" -v c="$total_cov_count" 'BEGIN{printf "%.2f%%", s/c}')"
  else
    avg_cov="NA"
  fi
  echo
  printf "Average line coverage over numeric entries: %s (from %d harnesses)\n" \
    "$avg_cov" "$total_cov_count"
  echo "============================================================"
} >> "$SUMMARY_OUT"

echo "📄 Coverage summary written to: $SUMMARY_OUT"
echo "📂 Temporary llvm-cov artifacts live under: $OUT_DIR"
