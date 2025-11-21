#!/usr/bin/env bash
set -euo pipefail

if [ $# -lt 1 ]; then
  echo "Usage: $0 <CWE_NUMBER>"
  exit 1
fi

CWE="$1"

HARNESS_DIR="gpt_sem_generated_test/CWE${CWE}"
JULIET_ROOT="data/juliet/testcases"
INCLUDE_JULIET="data/juliet/testcasesupport"

# Use Homebrew gcc/gcov if available
GCC_BIN="${GCC_BIN:-gcc-15}"
GCOV_BIN="${GCOV_BIN:-gcov-15}"

if ! command -v "$GCC_BIN" >/dev/null 2>&1; then
  GCC_BIN="gcc"
fi
if ! command -v "$GCOV_BIN" >/dev/null 2>&1; then
  GCOV_BIN="gcov"
fi

mkdir -p coverage_results

OUT_SUMMARY="coverage_results/cwe${CWE}_coverage.txt"

echo "🧪 [COVERAGE] Searching harnesses under $HARNESS_DIR"

HARNESS_LIST=$(find "$HARNESS_DIR" -type f -name '*_harness.c' | LC_ALL=C sort || true)
if [ -z "$HARNESS_LIST" ]; then
  echo "❌ No *_harness.c files found under $HARNESS_DIR"
  exit 1
fi

# Init summary file
: > "$OUT_SUMMARY"
printf "# Coverage results for CWE%s — %s\n" "$CWE" "$(date '+%Y-%m-%d %H:%M:%S')" >> "$OUT_SUMMARY"

total=0
ok=0
compile_fail=0
gcov_fail=0
nosrc=0

# Pre-resolve support sources (printLine, printIntLine, etc.)
SUPPORT_SOURCES=(data/juliet/testcasesupport/*.c)

for HARNESS in $HARNESS_LIST; do
  total=$((total+1))
  HARNESS_BASE=$(basename "$HARNESS")
  STEM="${HARNESS_BASE%_harness.c}"

  echo "👉 [$total] Harness: $HARNESS_BASE"

  # Find matching Juliet source
  JULIET_SOURCE=$(find "$JULIET_ROOT" -type f -name "${STEM}.c" | head -n 1 || true)
  if [ -z "$JULIET_SOURCE" ]; then
    echo "   ❌ Could not find Juliet source for ${STEM}.c"
    nosrc=$((nosrc+1))
    printf "[%s] CWE=%s | harness=%s | status=NO_SOURCE | coverage=0.0\n" \
      "$(date '+%Y-%m-%d %H:%M:%S')" "$CWE" "$HARNESS_BASE" >> "$OUT_SUMMARY"
    continue
  fi
  echo "   ✔ Juliet source: $JULIET_SOURCE"

  # Clean markdown fences into a temp harness
  CLEAN_HARNESS="$(mktemp)"
  sed 's/^```c$//; s/^```$//' "$HARNESS" > "$CLEAN_HARNESS"
  echo "   🧼 Cleaning harness (non-destructive)..."

  BIN="$(mktemp)"
  echo "   ⛏️  Compiling with coverage ($GCC_BIN)..."

  if ! "$GCC_BIN" \
      -I "$INCLUDE_JULIET" \
      -fprofile-arcs -ftest-coverage \
      "$CLEAN_HARNESS" "$JULIET_SOURCE" \
      "${SUPPORT_SOURCES[@]}" \
      -o "$BIN" >/dev/null 2>&1; then
    echo "   ❌ Compilation failed"
    compile_fail=$((compile_fail+1))
    printf "[%s] CWE=%s | harness=%s | status=COMPILE_FAIL | coverage=0.0\n" \
      "$(date '+%Y-%m-%d %H:%M:%S')" "$CWE" "$HARNESS_BASE" >> "$OUT_SUMMARY"
    rm -f "$CLEAN_HARNESS" "$BIN"
    continue
  fi

  echo "   ▶️  Running test binary..."
  "$BIN" >/dev/null 2>&1 || true

  echo "   📊  Running $GCOV_BIN..."
  # Run gcov on the Juliet source; capture stdout only
  GCOV_OUT="$("$GCOV_BIN" -o . "$JULIET_SOURCE" 2>/dev/null || true)"

  # Extract "Lines executed: XX.XX" from gcov output
  LINE_COV=$(printf '%s\n' "$GCOV_OUT" | awk '
    /^Lines executed:/ {
      # Example: Lines executed:85.71% of 14
      gsub("%","",$3);
      print $3;
      exit;
    }')

  if [ -z "$LINE_COV" ]; then
    echo "   ❌ Could not parse coverage from gcov output"
    gcov_fail=$((gcov_fail+1))
    printf "[%s] CWE=%s | harness=%s | status=GCOV_FAIL | coverage=0.0\n" \
      "$(date '+%Y-%m-%d %H:%M:%S')" "$CWE" "$HARNESS_BASE" >> "$OUT_SUMMARY"
  else
    echo "   ✅ Lines executed: ${LINE_COV}%"
    ok=$((ok+1))
    printf "[%s] CWE=%s | harness=%s | status=OK | coverage=%s\n" \
      "$(date '+%Y-%m-%d %H:%M:%S')" "$CWE" "$HARNESS_BASE" "$LINE_COV" >> "$OUT_SUMMARY"
  fi

  # Temp files are outside repo; removing is safe,
  # but you can comment these out if you want to inspect them.
  rm -f "$CLEAN_HARNESS" "$BIN"
done

{
  echo
  echo "================ Coverage Summary ================"
  printf "Total harnesses:      %d\n" "$total"
  printf "OK (coverage parsed): %d\n" "$ok"
  printf "Compile failures:     %d\n" "$compile_fail"
  printf "gcov parse failures:  %d\n" "$gcov_fail"
  printf "No source found:      %d\n" "$nosrc"
  echo "================================================="
} >> "$OUT_SUMMARY"

echo "📄 Coverage summary written to: $OUT_SUMMARY"
