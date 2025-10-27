#!/usr/bin/env bash
# Rebuild selected CWE(s) with AddressSanitizer + UBSanitizer
# Usage: ./scripts/rebuild_sanitized.sh 191 369 121 ...
set -euo pipefail
cd data/juliet
rm -rf bin/CWE{"$@"} || true
CFLAGS="-O0 -g -fsanitize=address,undefined -fno-omit-frame-pointer" \
CXXFLAGS="-O0 -g -fsanitize=address,undefined -fno-omit-frame-pointer" \
python3 juliet.py -c -g -m "$@"
echo "[✓] Rebuilt with sanitizers: $*"
