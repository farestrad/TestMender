#!/usr/bin/env bash
# =========================================================
# run_harness.sh — Compile & run generated Juliet harnesses (Bash 3 compatible)
# Usage:
#   ./scripts/run_harness.sh <CWE_NUMBER> <HARNESS_FILENAME> [--mode good|bad|both]
# Example:
#   ./scripts/run_harness.sh 121 CWE121_..._memcpy_09_harness.c --mode both
# Notes:
#   - --mode both (default): runs harness main, expects BAD to trigger ASan.
#   - --mode good|bad: builds a tiny main to call only good() or bad() so runs are isolated.
# =========================================================
set -euo pipefail

if [ $# -lt 2 ]; then
  echo "Usage: $0 <CWE_NUMBER> <HARNESS_FILENAME> [--mode good|bad|both]"
  exit 1
fi

CWE="$1"
HARNESS_NAME="$2"
MODE="both"
if [ $# -ge 3 ]; then
  case "$3" in
    --mode)
      echo "ERROR: use '--mode good|bad|both' (single flag)"; exit 1;;
    --mode=good|--mode=bad|--mode=both)
      MODE="${3#--mode=}";;
    *)
      echo "Unknown option: $3"; exit 1;;
  esac
fi

JULIET_BASE="data/juliet/testcases"
SUPPORT_DIR="data/juliet/testcasesupport"
HAR_PATH="generated_test_cases/CWE${CWE}/${HARNESS_NAME}"
BUILD_DIR="build"
mkdir -p "$BUILD_DIR"

if [ ! -d "$JULIET_BASE" ]; then
  echo "❌ Juliet base not found: $JULIET_BASE"
  exit 1
fi
if [ ! -f "$HAR_PATH" ]; then
  echo "❌ Harness not found: $HAR_PATH"
  exit 1
fi

# 1) Locate matching Juliet sources
STEM="${HARNESS_NAME%%_harness.c}"
PRIMARY_C="${STEM}.c"
BASENAME="${STEM##*/}"

echo "🔎 Looking for sources:"
echo "   JULIET_BASE: $JULIET_BASE"
echo "   Exact filename: $PRIMARY_C"

SRC_FILES=()
while IFS= read -r -d '' f; do
  SRC_FILES[${#SRC_FILES[@]}]="$f"
done < <(find "$JULIET_BASE" -type f -name "$PRIMARY_C" -print0)

if [ ${#SRC_FILES[@]} -eq 0 ]; then
  echo "   (no exact match) Trying family pattern: ${STEM}_*.c"
  while IFS= read -r -d '' f; do
    SRC_FILES[${#SRC_FILES[@]}]="$f"
  done < <(find "$JULIET_BASE" -type f -name "${STEM}_*.c" -print0)
fi

if [ ${#SRC_FILES[@]} -eq 0 ]; then
  echo "   (no family match) Trying loose fallback: *${BASENAME}.c"
  while IFS= read -r -d '' f; do
    SRC_FILES[${#SRC_FILES[@]}]="$f"
  done < <(find "$JULIET_BASE" -type f -name "*${BASENAME}.c" -print0)
fi

if [ ${#SRC_FILES[@]} -eq 0 ]; then
  echo "❌ No Juliet source files found for stem: $STEM"
  echo "ℹ️  Try manually: find $JULIET_BASE -type f -name \"$PRIMARY_C\""
  exit 1
fi

echo "📄 Found ${#SRC_FILES[@]} Juliet file(s):"
for f in "${SRC_FILES[@]}"; do echo "   - $f"; done

echo "📦 Compiling harness with AddressSanitizer..."

# 2) Resolve *_good / *_bad symbols robustly
GOOD_SYM="${STEM}_good"
BAD_SYM="${STEM}_bad"

symbol_exists() {
  local sym="$1"
  LC_ALL=C grep -Eq '^[[:space:]]*[A-Za-z_][A-Za-z0-9_[:space:]\*\(\)]*[[:space:]]+'"$sym"'[[:space:]]*\(' "${SRC_FILES[@]}" 2>/dev/null
}
extract_first_suffix_fn() {
  local suffix="$1"
  LC_ALL=C grep -hE '^[[:space:]]*[A-Za-z_][A-Za-z0-9_[:space:]\*\(\)]*[[:space:]]+[A-Za-z_][A-Za-z0-9_]*_'"$suffix"'[[:space:]]*\(' "${SRC_FILES[@]}" 2>/dev/null \
    | sed -E 's/.*[[:space:]]+([A-Za-z_][A-Za-z0-9_]*)_'"$suffix"'[[:space:]]*\(.*/\1_'"$suffix"'/' \
    | head -n1
}

if ! symbol_exists "$GOOD_SYM"; then
  maybe=$(extract_first_suffix_fn good || true); [ -n "${maybe:-}" ] && GOOD_SYM="$maybe"
fi
if ! symbol_exists "$BAD_SYM"; then
  maybe=$(extract_first_suffix_fn bad || true);  [ -n "${maybe:-}" ] && BAD_SYM="$maybe"
fi

echo "🔧 Resolved symbols:"
echo "   good(): ${GOOD_SYM:-<unresolved>}"
echo "   bad():  ${BAD_SYM:-<unresolved>}"

SHIM_C="$BUILD_DIR/${STEM}_shim.c"
MAIN_C="$BUILD_DIR/${STEM}_main.c"

# 3) Emit shim that maps good()/bad() to actual Juliet symbols if present
{
  echo '#include <stdio.h>'
  if [ -n "${GOOD_SYM:-}" ] && symbol_exists "$GOOD_SYM"; then
    echo "extern void ${GOOD_SYM}(void);"
    echo "void good(void) { ${GOOD_SYM}(); }"
  else
    echo 'void good(void) { fprintf(stderr, "good() shim unresolved\n"); __builtin_trap(); }'
  fi
  if [ -n "${BAD_SYM:-}" ] && symbol_exists "$BAD_SYM"; then
    echo "extern void ${BAD_SYM}(void);"
    echo "void bad(void)  { ${BAD_SYM}(); }"
  else
    echo 'void bad(void)  { fprintf(stderr, "bad() shim unresolved\n"); __builtin_trap(); }'
  fi
} > "$SHIM_C"

# 4) Optionally emit a targeted main to call only good() or bad()
TARGET_MAIN=0
if [ "$MODE" != "both" ]; then
  TARGET_MAIN=1
  {
    echo '#include <stdio.h>'
    echo 'void good(void); void bad(void);'
    if [ "$MODE" = "good" ]; then
      echo 'int main(void){ good(); puts("GOOD: PASS"); return 0; }'
    else
      echo 'int main(void){ bad();  puts("BAD:  DONE"); return 0; }'
    fi
  } > "$MAIN_C"
fi

OUT_BIN="$BUILD_DIR/${STEM}_asan"
CLANG_INPUTS=( "${SRC_FILES[@]}" "$SUPPORT_DIR"/*.c "$HAR_PATH" "$SHIM_C" )
if [ $TARGET_MAIN -eq 1 ]; then
  CLANG_INPUTS=( "${SRC_FILES[@]}" "$SUPPORT_DIR"/*.c "$MAIN_C" "$SHIM_C" )
fi

clang -fsanitize=address -fno-omit-frame-pointer -O0 -g \
  -I "$SUPPORT_DIR" \
  "${CLANG_INPUTS[@]}" \
  -o "$OUT_BIN"

# 5) Run (capture & grade)
echo "🚀 Running..."
LOG="$BUILD_DIR/${STEM}_run.log"
ASAN_OPTIONS="detect_leaks=0:halt_on_error=1:abort_on_error=1:allocator_may_return_null=1"

set +e
ASAN_OPTIONS="$ASAN_OPTIONS" "$OUT_BIN" >"$LOG" 2>&1
status=$?
set -e

if grep -q "AddressSanitizer" "$LOG"; then
  if [ "$MODE" = "bad" ] || [ "$MODE" = "both" ]; then
    echo "🧪 ASan error detected (expected for BAD path) — PASS"
    echo "   (See log: $LOG)"
    exit 0
  else
    echo "❌ ASan error while running GOOD path — FAIL"
    tail -n 50 "$LOG"; exit 2
  fi
fi

if [ $status -eq 0 ]; then
  echo "✅ Clean run (no ASan findings) — PASS"
  echo "   (See log: $LOG)"
  exit 0
fi

echo "❌ Non-ASan failure (exit $status). Showing tail of log:"
tail -n 50 "$LOG"
exit $status
