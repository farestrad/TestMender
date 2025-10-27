#!/usr/bin/env bash
set -euo pipefail

# Usage: scripts/validate_diff.sh <bad.c> <good.c> <harness.c>
bad="$1"
good="$2"
harness="$3"

mkdir -p build

echo "[*] Compiling objects..."
clang -O0 -g -c "$bad"    -DOMITGOOD -o build/bad.o
clang -O0 -g -c "$good"   -DOMITBAD  -o build/good.o
clang -O0 -g -c "$harness"            -o build/harness.o

echo "[*] Linking executables..."
clang -O0 -g -o build/run_bad  build/harness.o build/bad.o
clang -O0 -g -o build/run_good build/harness.o build/good.o

echo "[*] Running..."
set +e
./build/run_bad;  bad_rc=$?
./build/run_good; good_rc=$?
set -e

echo "bad=$bad_rc  good=$good_rc"
if [[ "$bad_rc" -ne 0 && "$good_rc" -eq 0 ]]; then
  echo "✅ Differential correctness: PASS"
  exit 0
else
  echo "❌ Differential correctness: FAIL (expected bad!=0 and good==0)"
  exit 1
fi