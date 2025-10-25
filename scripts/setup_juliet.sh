#!/usr/bin/env bash
set -euo pipefail

DEST="${JULIET_DIR:-data/juliet}"
URL_DEFAULT="${JULIET_URL:-}"
ARCHIVE="${1:-}"

mkdir -p "$DEST"
tmp="$(mktemp -d)"; trap 'rm -rf "$tmp"' EXIT

if [[ -n "$ARCHIVE" && -f "$ARCHIVE" ]]; then
  echo "[*] Using local archive: $ARCHIVE"
elif [[ -n "$ARCHIVE" && "$ARCHIVE" =~ ^https?:// ]]; then
  echo "[*] Downloading: $ARCHIVE"
  curl -L --fail -o "$tmp/juliet_archive" "$ARCHIVE"
  ARCHIVE="$tmp/juliet_archive"
elif [[ -n "$URL_DEFAULT" ]]; then
  echo "[*] Downloading JULIET_URL: $URL_DEFAULT"
  curl -L --fail -o "$tmp/juliet_archive" "$URL_DEFAULT"
  ARCHIVE="$tmp/juliet_archive"
else
  cat <<USAGE
Usage:
  scripts/setup_juliet.sh /path/to/Juliet-archive.(zip|tar.gz)
  scripts/setup_juliet.sh https://example.com/Juliet-archive.(zip|tar.gz)

Or set env:
  export JULIET_DIR=data/juliet
  export JULIET_URL=https://...
  scripts/setup_juliet.sh
USAGE
  exit 1
fi

echo "[*] Extracting to $DEST ..."
case "$ARCHIVE" in
  *.tar.gz|*.tgz|*/juliet_archive)
    tar -xzf "$ARCHIVE" -C "$DEST" || tar -xzf "$ARCHIVE" -C "$DEST"
    ;;
  *.zip)
    unzip -q "$ARCHIVE" -d "$DEST"
    ;;
  *)
    echo "Unknown archive type: $ARCHIVE"; exit 1 ;;
esac

echo "[✓] Juliet ready in: $DEST (git-ignored)"
