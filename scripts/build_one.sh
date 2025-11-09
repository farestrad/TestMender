#!/usr/bin/env bash
set -euo pipefail

CSV="config/pilot_manifest.csv"
PID="${1:-}"
if [[ -z "$PID" ]]; then
  echo "usage: $0 pilot_v1_0007" >&2
  exit 1
fi

# Extract fields from the CSV (by pilot_id)
line=$(awk -F, -v id="$PID" 'NR>1 && $1==id{print; exit}' "$CSV")
if [[ -z "$line" ]]; then
  echo "pilot_id not found: $PID" >&2
  exit 1
fi

# Columns:
# 1:pilot_id 2:cwe 3:case_id 4:src_file 5:good_func 6:bad_func
# 7:build_flags 8:run_args 9:oracle_type 10:oracle_value 11:notes
IFS=, read -r pilot_id cwe case_id src_file good_func bad_func build_flags run_args oracle_type oracle_value notes <<<"$line"

# Paths
ROOT="$(pwd)"
SUPPORT_DIR="data/juliet/testcasesupport"
OUT_DIR="artifacts/pilot_v1"
BIN="$OUT_DIR/${pilot_id}.out"

mkdir -p "$OUT_DIR"

# Gather support sources (Juliet needs these)
# exclude any main.c variants
# Gather support sources (Juliet needs these)
# exclude any main.c variants – use a POSIX-compatible approach
SUPPORT_SOURCES=$(find "$SUPPORT_DIR" -type f -name '*.c' ! -name 'main.c')


# Build
echo "[build] $pilot_id → $BIN"
# shellcheck disable=SC2086
gcc $build_flags \
  -I"$SUPPORT_DIR" -I"$SUPPORT_DIR/io" \
  $src_file $SUPPORT_SOURCES \
  -o "$BIN"

# Run
echo "[run] $BIN $run_args"
set +e
"$BIN" $run_args
status=$?
set -e

echo "[status] exit_code=$status  oracle_type=$oracle_type oracle_value=$oracle_value"
