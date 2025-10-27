#!/usr/bin/env bash
set -euo pipefail

DEST="data/juliet"

if [ ! -d "$DEST/.git" ]; then
  echo "[*] Cloning Juliet Test Suite (C/C++) from GitHub..."
  git clone --depth 1 https://github.com/arichardson/juliet-test-suite-c.git "$DEST"
else
  echo "[*] Juliet already exists. Pulling latest changes..."
  git -C "$DEST" pull
fi

echo "[✓] Juliet dataset ready in $DEST (git-ignored)."
