#!/usr/bin/env bash

set -euo pipefail

print_usage() {
  echo "Usage: $0 <CWE_NUMBER> <HARNESS_FILENAME> [--mode=good|bad|both] [--quiet] [--emit=csv] [--timeout=SECS]"
}

# ---- Arg parsing ----
if [ $# -lt 2 ]; then
  print_usage
  exit 1
fi

CWE="$1"; shift
HARNESS_NAME="$1"; shift

MODE="both"
QUIET=0
EMIT="none"   # csv | none
HARNESS_TIMEOUT=15  # seconds default

while [ $# -gt 0 ]; do
  case "$1" in
    --mode=good) MODE="good";;
    --mode=bad)  MODE="bad";;
    --mode=both) MODE="both";;
    --mode)
      shift || true
      case "${1:-}" in
        good|bad|both) MODE="$1";;
        *) echo "ERROR: --mode requires good|bad|both"; exit 1;;
      esac
      ;;
    --quiet) QUIET=1;;
    --emit=csv) EMIT="csv";;
    --emit)
      shift || true
      [ "${1:-}" = "csv" ] && EMIT="csv" || { echo "ERROR: --emit requires 'csv'"; exit 1; }
      ;;
    --timeout=*)
      HARNESS_TIMEOUT="${1#--timeout=}";;
    --timeout)
      shift || true
      HARNESS_TIMEOUT="${1:-15}"
      ;;
    *)
      echo "Unknown option: $1"
      print_usage
      exit 1
      ;;
  esac
  shift || true
done

# ---- Timeout helper (macOS-safe) ----
run_with_timeout() {
  # $1 timeout seconds, rest is command
  local secs="$1"; shift
  if command -v gtimeout >/dev/null 2>&1; then
    gtimeout "$secs" "$@"
    return $?
  elif command -v timeout >/dev/null 2>&1; then
    timeout "$secs" "$@"
    return $?
  elif command -v perl >/dev/null 2>&1; then
    perl -e 'alarm shift @ARGV; exec @ARGV' "$secs" "$@"
    return $?
  else
    # No timeout available; run directly
    "$@"
    return $?
  fi
}

# ---- Paths ----
JULIET_BASE="data/juliet/testcases"
SUPPORT_DIR="data/juliet/testcasesupport"
HARNESS_BASE_DIR="${HARNESS_BASE_DIR:-generated_test_cases}"
HAR_PATH="${HARNESS_BASE_DIR}/CWE${CWE}/${HARNESS_NAME}"
BUILD_DIR="build"
mkdir -p "$BUILD_DIR"

[ -d "$JULIET_BASE" ]  || { echo "❌ Juliet base not found: $JULIET_BASE"; exit 1; }
[ -d "$SUPPORT_DIR" ]  || { echo "❌ Juliet support dir not found: $SUPPORT_DIR"; exit 1; }
[ -f "$HAR_PATH" ]     || { echo "❌ Harness not found: $HAR_PATH"; exit 1; }

# ---- MODE=both wrapper (run good then bad) ----
if [ "$MODE" = "both" ]; then
  extra_args=()
  [ $QUIET -eq 1 ] && extra_args+=(--quiet)
  [ "$EMIT" = "csv" ] && extra_args+=(--emit=csv)
  extra_args+=(--timeout="$HARNESS_TIMEOUT")

  good_rc=0
  bad_rc=0

  "$0" "$CWE" "$HARNESS_NAME" --mode=good "${extra_args[@]}" || good_rc=$?
  "$0" "$CWE" "$HARNESS_NAME" --mode=bad  "${extra_args[@]}" || bad_rc=$?

  if [ $good_rc -eq 0 ] && [ $bad_rc -eq 0 ]; then
    exit 0
  else
    exit 1
  fi
fi

# ---- Derive STEM and Juliet filename ----
# HARNESS_NAME examples:
#   CWE121_..._67b_harness.c
#   CWE121_..._67b_harness_improved.c
# We want STEM = CWE121_..._67b
BASE_NO_EXT="${HARNESS_NAME%.c}"          # drop trailing .c if present
STEM="${BASE_NO_EXT%_harness_improved}"   # remove _harness_improved if present
STEM="${STEM%_harness}"                   # remove _harness if present
PRIMARY_C="${STEM}.c"
BASENAME="${STEM##*/}"

[ $QUIET -eq 1 ] || {
  echo "🔎 Looking for sources:"
  echo "   JULIET_BASE: $JULIET_BASE"
  echo "   Primary Juliet filename: $PRIMARY_C"
}

# ---- Locate Juliet source files ----
SRC_FILES=()

_find_list() {
  find "$1" -type f -name "$2" 2>/dev/null | LC_ALL=C sort
}

# Exact match
while IFS= read -r f; do
  [ -n "$f" ] && SRC_FILES+=("$f")
done < <(_find_list "$JULIET_BASE" "$PRIMARY_C")

# Family pattern: STEM_*.c
if [ ${#SRC_FILES[@]} -eq 0 ]; then
  [ $QUIET -eq 1 ] || echo "   (no exact match) Trying family pattern: ${STEM}_*.c"
  while IFS= read -r f; do
    [ -n "$f" ] && SRC_FILES+=("$f")
  done < <(_find_list "$JULIET_BASE" "${STEM}_*.c")
fi

# Loose fallback: *BASENAME.c
if [ ${#SRC_FILES[@]} -eq 0 ]; then
  [ $QUIET -eq 1 ] || echo "   (no family match) Trying loose fallback: *${BASENAME}.c"
  while IFS= read -r f; do
    [ -n "$f" ] && SRC_FILES+=("$f")
  done < <(_find_list "$JULIET_BASE" "*${BASENAME}.c")
fi

[ ${#SRC_FILES[@]} -gt 0 ] || {
  echo "❌ No Juliet source files found for stem: $STEM"
  exit 4
}

# ---- Compile harness + Juliet + support with ASan ----
OUT_BIN="$BUILD_DIR/${STEM}_asan_${MODE}"
LOG="$BUILD_DIR/${STEM}_run_${MODE}.log"

CLANG_DEFS=()
if [ "$MODE" = "bad" ]; then
  # Harness can use #ifdef TEST_MODE_BAD to switch behavior
  CLANG_DEFS+=("-DTEST_MODE_BAD=1")
fi

[ $QUIET -eq 1 ] || {
  echo "🛠  Compiling:"
  echo "   Output: $OUT_BIN"
  echo "   Harness: $HAR_PATH"
  printf "   Juliet sources (%d):\n" "${#SRC_FILES[@]}"
  for f in "${SRC_FILES[@]}"; do
    echo "     - $f"
  done
}

if [ "${#CLANG_DEFS[@]:-0}" -gt 0 ]; then
  clang \
    -fsanitize=address -fno-omit-frame-pointer -O0 -g \
    -I "$SUPPORT_DIR" \
    "${CLANG_DEFS[@]}" \
    "${SRC_FILES[@]}" \
    "$SUPPORT_DIR"/*.c \
    "$HAR_PATH" \
    -o "$OUT_BIN"
else
  clang \
    -fsanitize=address -fno-omit-frame-pointer -O0 -g \
    -I "$SUPPORT_DIR" \
    "${SRC_FILES[@]}" \
    "$SUPPORT_DIR"/*.c \
    "$HAR_PATH" \
    -o "$OUT_BIN"
fi


# ---- Run under ASan with timeout ----
ASAN_OPTIONS="detect_leaks=0:halt_on_error=1:abort_on_error=1:allocator_may_return_null=1"

set +e
run_with_timeout "$HARNESS_TIMEOUT" env ASAN_OPTIONS="$ASAN_OPTIONS" "$OUT_BIN" >"$LOG" 2>&1
status=$?
set -e

ASAN_SEEN="no"
if grep -q "AddressSanitizer" "$LOG"; then
  ASAN_SEEN="yes"
fi

STATUS_TXT="FAIL"
rc=2

if [ "$MODE" = "bad" ]; then
  # BAD: we EXPECT ASan to fire
  if [ "$ASAN_SEEN" = "yes" ]; then
    STATUS_TXT="PASS"
    rc=0
  else
    STATUS_TXT="FAIL"
    rc=$status
  fi
else
  # GOOD: we expect clean run, no ASan, exit=0
  if [ "$ASAN_SEEN" = "no" ] && [ $status -eq 0 ]; then
    STATUS_TXT="PASS"
    rc=0
  else
    STATUS_TXT="FAIL"
    rc=$status
  fi
fi

# ---- Human-readable output ----
if [ $QUIET -eq 0 ]; then
  echo "Mode: $MODE"
  if [ "$STATUS_TXT" = "PASS" ]; then
    if [ "$MODE" = "bad" ]; then
      echo "🧪 ASan error detected (expected for BAD path) — PASS"
    else
      echo "✅ Clean run (no ASan findings) — PASS"
    fi
  else
    if [ "$MODE" = "bad" ]; then
      echo "❌ BAD path expected ASan, but run was clean or crashed unexpectedly — FAIL"
    else
      echo "❌ GOOD path had error or ASan finding — FAIL"
    fi
    echo "---- Last 50 lines of log ($LOG) ----"
    tail -n 50 "$LOG" || true
  fi
fi

# ---- CSV output for batch scripts ----
if [ "$EMIT" = "csv" ]; then
  # cwe,harness,mode,status,asan,exit_code,logpath
  printf "%s,%s,%s,%s,%s,%s,%s\n" \
    "$CWE" "$HARNESS_NAME" "$MODE" "$STATUS_TXT" "$ASAN_SEEN" "$status" "$LOG"
fi

exit $rc
