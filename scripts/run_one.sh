#!/usr/bin/env bash
# =========================================================
# run_one.sh — run one Juliet CWE pair (bad vs good)
# Usage: ./scripts/run_one.sh <CWE_NUMBER>
# Example: ./scripts/run_one.sh 191
# =========================================================
set -euo pipefail

CWE="${1:-}"
if [[ -z "$CWE" ]]; then
  echo "Usage: $0 <CWE_NUMBER>"
  exit 1
fi

BASE="data/juliet/bin/CWE${CWE}"

if [[ ! -d "$BASE" ]]; then
  echo "CWE${CWE} not found under $BASE"
  exit 1
fi

# Pick the first BAD executable found
BAD_EXE=$(find "$BASE/bad" -type f | head -n 1 || true)
if [[ -z "$BAD_EXE" ]]; then
  echo "No bad executables found for CWE${CWE}"
  exit 1
fi

# Derive matching GOOD executable
GOOD_EXE=$(echo "$BAD_EXE" | sed -e 's:/bad/:/good/:' -e 's:-bad$:-good:')

echo "BAD  = $BAD_EXE"
echo "GOOD = $GOOD_EXE"
echo "-------------------------------------"

if [[ ! -x "$GOOD_EXE" ]]; then
  echo "Warning: GOOD executable missing or not executable"
fi

# Run each and capture return codes
set +e
"$BAD_EXE"
bad_rc=$?
"$GOOD_EXE"
good_rc=$?
set -e

echo "bad=$bad_rc  good=$good_rc"
