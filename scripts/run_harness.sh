#!/usr/bin/env bash
# =========================================================
# run_harness.sh — Compile & run generated Juliet harnesses (Bash 3 compatible)
# Usage:
#   ./scripts/run_harness.sh <CWE_NUMBER> <HARNESS_FILENAME>
# Example:
#   ./scripts/run_harness.sh 121 CWE121_Stack_Based_Buffer_Overflow__char_type_overrun_memcpy_09_harness.c
# =========================================================
set -euo pipefail

if [ $# -lt 2 ]; then
  echo "Usage: $0 <CWE_NUMBER> <HARNESS_FILENAME>"
  exit 1
fi

CWE="$1"
HARNESS_NAME="$2"

JULIET_BASE="data/juliet/testcases"
SUPPORT_DIR="data/juliet/testcasesupport"
HAR_PATH="generated_test_cases/CWE${CWE}/${HARNESS_NAME}"
BUILD_DIR="build"
mkdir -p "$BUILD_DIR"

# 1) Locate matching Juliet sources
STEM="${HARNESS_NAME%%_harness.c}"      # strip suffix
PRIMARY_C="${STEM}.c"

SRC_FILES=()

# helper: add files from a find command (-print0)
add_found_files() {
  while IFS= read -r -d '' f; do
    SRC_FILES[${#SRC_FILES[@]}]="$f"
  done
}

# try exact file
find "$JULIET_BASE" -type f -name "$PRIMARY_C" -print0 | add_found_files

# if none, try family files (54a..54e, etc.)
if [ ${#SRC_FILES[@]} -eq 0 ]; then
  find "$JULIET_BASE" -type f -name "${STEM}_*.c" -print0 | add_found_files
fi

if [ ${#SRC_FILES[@]} -eq 0 ]; then
  echo "❌ No Juliet source files found for stem: $STEM"
  exit 1
fi

echo "📦 Compiling harness with ${#SRC_FILES[@]} Juliet file(s)..."

# 2) Auto-detect real *_good / *_bad symbols
GOOD_SYM=""
BAD_SYM=""

for f in "${SRC_FILES[@]}"; do
  if [ -z "$GOOD_SYM" ]; then
    g=$(LC_ALL=C grep -E '^[[:space:]]*[a-zA-Z_][a-zA-Z0-9_[:space:]\*\(\)]*[[:space:]]+([A-Za-z0-9_]+)_good[[:space:]]*\(' "$f" \
        | sed -E 's/.*[^A-Za-z0-9_]?([A-Za-z0-9_]+)_good[[:space:]]*\(.*/\1_good/' | head -n1 || true)
    [ -n "$g" ] && GOOD_SYM="$g"
  fi
  if [ -z "$BAD_SYM" ]; then
    b=$(LC_ALL=C grep -E '^[[:space:]]*[a-zA-Z_][a-zA-Z0-9_[:space:]\*\(\)]*[[:space:]]+([A-Za-z0-9_]+)_bad[[:space:]]*\(' "$f" \
        | sed -E 's/.*[^A-Za-z0-9_]?([A-Za-z0-9_]+)_bad[[:space:]]*\(.*/\1_bad/' | head -n1 || true)
    [ -n "$b" ] && BAD_SYM="$b"
  fi
  [ -n "$GOOD_SYM" ] && [ -n "$BAD_SYM" ] && break
done

# 3) Emit shim to map generic good()/bad() to real Juliet symbols
SHIM_C="$BUILD_DIR/${STEM}_shim.c"
{
  echo '#include <stdio.h>'
  if [ -n "$GOOD_SYM" ]; then
    echo "extern void ${GOOD_SYM}(void);"
    echo "void good(void) { ${GOOD_SYM}(); }"
  else
    echo 'void good(void) { fprintf(stderr, "good() shim unresolved\n"); __builtin_trap(); }'
  fi
  if [ -n "$BAD_SYM" ]; then
    echo "extern void ${BAD_SYM}(void);"
    echo "void bad(void)  { ${BAD_SYM}(); }"
  else
    echo 'void bad(void)  { fprintf(stderr, "bad() shim unresolved\n"); __builtin_trap(); }'
  fi
} > "$SHIM_C"

# 4) Compile with ASan
OUT_BIN="$BUILD_DIR/${STEM}_asan"
clang -fsanitize=address -fno-omit-frame-pointer -O0 -g \
  -I "$SUPPORT_DIR" \
  "${SRC_FILES[@]}" "$SUPPORT_DIR"/*.c "$HAR_PATH" "$SHIM_C" \
  -o "$OUT_BIN"

echo "🚀 Running..."
"$OUT_BIN"
