#!/usr/bin/env bash
# =========================================================
# run_cppcheck.sh — Run Cppcheck on specified CWE(s)
# Usage: ./scripts/run_cppcheck.sh <CWE_NUMBER> [CWE_NUMBER...]
# Example: ./scripts/run_cppcheck.sh 121 122 190
# =========================================================
set -euo pipefail

# --- Guards ------------------------------------------------
if ! command -v cppcheck >/dev/null 2>&1; then
  echo "❌ Cppcheck is not installed. Install via Homebrew (brew install cppcheck) or apt (sudo apt install cppcheck)."
  exit 1
fi

if [[ $# -eq 0 ]]; then
  echo "Usage: $0 <CWE_NUMBER> [CWE_NUMBER...]"
  echo "Example: $0 121 122 190"
  exit 1
fi

# --- Paths & Output ---------------------------------------
OUTPUT_BASE="analysis_results/cppcheck"
mkdir -p "$OUTPUT_BASE"

TIMESTAMP=$(date +"%Y%m%d_%H%M%S")

echo "=========================================="
echo "🔍 Running Cppcheck Analysis"
echo "=========================================="
echo "CWEs to analyze: $*"
echo "Output directory: $OUTPUT_BASE"
echo "=========================================="

# --- Helper: find CWE dir ---------------------------------
find_cwe_dir() {
  local cwe_num="$1"
  local testcases_base="data/juliet/testcases"
  local found_dir
  found_dir=$(find "$testcases_base" -maxdepth 1 -type d -name "CWE${cwe_num}*" | head -1 || true)
  [[ -n "${found_dir:-}" ]] && echo "$found_dir"
}

# --- Scan loop --------------------------------------------
for CWE in "$@"; do
  SOURCE_DIR="$(find_cwe_dir "$CWE" || true)"

  if [[ -z "${SOURCE_DIR:-}" || ! -d "$SOURCE_DIR" ]]; then
    echo ""
    echo "⚠️  Warning: CWE${CWE} source directory not found"
    echo "   Searched in: data/juliet/testcases/CWE${CWE}*"
    echo "   Skipping..."
    continue
  fi

  # --- Adjusted output structure ---------------------------------
  BASE_DIR="$OUTPUT_BASE/CWE${CWE}_${TIMESTAMP}"
  FULL_DIR="$BASE_DIR/full_results"
  INDIVIDUAL_DIR="$BASE_DIR/individual"

  mkdir -p "$FULL_DIR" "$INDIVIDUAL_DIR"

  XML_OUTPUT="$FULL_DIR/CWE${CWE}_${TIMESTAMP}.xml"
  JSON_OUTPUT="$FULL_DIR/CWE${CWE}_${TIMESTAMP}.json"

  echo ""
  echo "📊 Analyzing CWE${CWE}..."
  echo "   Source: $SOURCE_DIR"
  echo "   Output (JSON): $JSON_OUTPUT"

  # --- Cppcheck run (XML output) ----------------------------
  echo "   Running Cppcheck..."
  if cppcheck --enable=all \
      --xml \
      --xml-version=2 \
      --suppress=missingIncludeSystem \
      --quiet \
      "$SOURCE_DIR" 2>"$XML_OUTPUT"; then
    :
  else
    if [[ ! -s "$XML_OUTPUT" ]]; then
      echo "   ❌ Cppcheck failed and produced no output"
      continue
    else
      echo "   ⚠️  Cppcheck returned non-zero (often normal); output captured."
    fi
  fi

  # --- Convert XML to JSON ----------------------------------
  python3 - <<EOF
import xml.etree.ElementTree as ET
import json
import sys

try:
    tree = ET.parse('$XML_OUTPUT')
    root = tree.getroot()

    results = {
        'tool': 'cppcheck',
        'cwe': '$CWE',
        'source_dir': '$SOURCE_DIR',
        'timestamp': '$TIMESTAMP',
        'findings': []
    }

    for error in root.findall('.//error'):
        finding = {
            'id': error.get('id', ''),
            'severity': error.get('severity', ''),
            'msg': error.get('msg', ''),
            'verbose': error.get('verbose', ''),
            'cwe': error.get('cwe', ''),
            'locations': []
        }

        for location in error.findall('location'):
            finding['locations'].append({
                'file': location.get('file', ''),
                'line': location.get('line', ''),
                'column': location.get('column', ''),
                'info': location.get('info', '')
            })

        results['findings'].append(finding)

    with open('$JSON_OUTPUT', 'w') as f:
        json.dump(results, f, indent=2)

    print(f"   ✅ Analysis complete")
    print(f"   📈 Found {len(results['findings'])} issue(s)")

except Exception as e:
    print(f"   ❌ Error converting XML to JSON: {e}", file=sys.stderr)
    sys.exit(1)
EOF

  # --- Summaries & per-file breakdown ----------------------
  if [[ -f "$JSON_OUTPUT" ]]; then
    if command -v jq >/dev/null 2>&1; then
      FINDING_COUNT=$(jq '.findings | length' "$JSON_OUTPUT" 2>/dev/null || echo "0")
      echo "   📈 Total findings: $FINDING_COUNT"

      echo "------------------------------------------"
      echo "📂 Per-file summary:"
      jq -r '
        .findings[] as $finding
        | $finding.locations[]?
        | "\(.file) → [\($finding.severity)] \($finding.id)"
      ' "$JSON_OUTPUT" | sort | uniq | head -20

      echo "------------------------------------------"
      echo "📊 Top files by finding count:"
      TSV_TABLE=$(
        jq -r '
          [.findings[].locations[]? | .file]
          | group_by(.)
          | map({file: .[0], count: length})
          | sort_by(-.count)[:20]
          | (["count","file"]),
            (.[] | [(.count|tostring), .file])
          | @tsv
        ' "$JSON_OUTPUT"
      )
      if command -v column >/dev/null 2>&1; then
        echo "$TSV_TABLE" | column -t
      else
        echo "$TSV_TABLE"
      fi

      # CSV summary lives next to the full JSON (in full_results/)
      CSV_FILE="${JSON_OUTPUT%.json}.summary.csv"
      jq -r '
        [.findings[].locations[]? | .file]
        | group_by(.)
        | map({file: .[0], count: length})
        | sort_by(-.count)
        | (["file","count"]),
          (.[] | [.file, (.count|tostring)])
        | @csv
      ' "$JSON_OUTPUT" > "$CSV_FILE"
      echo "🧾 Wrote CSV summary: $CSV_FILE"

      echo "------------------------------------------"
      echo "🧪 Function/file hint (GOOD vs BAD):"
      if command -v rg >/dev/null 2>&1; then
        while IFS= read -r f; do
          [[ -z "$f" ]] && continue
          if [[ "$f" =~ [Gg][Oo][Oo][Dd] ]] || rg -q --no-mmap --fixed-strings "void good" "$f" 2>/dev/null; then
            echo "✅ GOOD: $f"
          elif [[ "$f" =~ [Bb][Aa][Dd] ]] || rg -q --no-mmap --fixed-strings "void bad" "$f" 2>/dev/null; then
            echo "❌ BAD:  $f"
          else
            echo "⚙️  MIXED/UNKNOWN: $f"
          fi
        done < <(jq -r '[.findings[].locations[]?.file] | unique[]' "$JSON_OUTPUT" 2>/dev/null)
      else
        jq -r '
          [.findings[].locations[]?.file] | unique[]
          | if (. | test("good"; "i")) then
              "✅ GOOD: " + .
            elif (. | test("bad"; "i")) then
              "❌ BAD:  " + .
            else
              "⚙️  MIXED/UNKNOWN: " + .
            end
        ' "$JSON_OUTPUT" | head -20
      fi

           # Per-file JSON split → individual/
      echo "------------------------------------------"
      echo "🗂  Writing per-file JSONs to: $INDIVIDUAL_DIR/"

      FILES_LIST="$(jq -r '[.findings[].locations[]?.file] | unique[]' "$JSON_OUTPUT" 2>/dev/null || true)"
      while IFS= read -r f; do
        [[ -z "$f" ]] && continue
        safe_name="$(echo "$f" | tr '/ ' '__')"
        per_file="$INDIVIDUAL_DIR/${safe_name}.json"

        jq --arg filepath "$f" '
          {
            tool,
            cwe,
            path: $filepath,
            findings: [
              .findings[]
              | select(
                  # keep this finding if any of its locations has this exact file
                  [ .locations[]?.file ] | index($filepath)
                )
            ]
          }
        ' "$JSON_OUTPUT" > "$per_file"

        # Only keep non-empty files
        if [[ "$(jq '.findings | length' "$per_file")" -gt 0 ]]; then
          echo "   • $(basename "$per_file") ($(jq ".findings | length" "$per_file") findings)"
        else
          rm -f "$per_file"
        fi
      done <<< "$FILES_LIST"

      echo "------------------------------------------"

    else
      echo "   📈 Install 'jq' to see summaries, tables, and per-file outputs"
    fi
  else
    echo "   ❌ Output file not created"
  fi
done

echo ""
echo "=========================================="
echo "✅ Cppcheck Analysis Complete"
echo "=========================================="
echo ""
echo "📁 Results saved in: $OUTPUT_BASE/"
echo ""
echo "🔍 View results:"
for CWE in "$@"; do
  BASE_DIR="$OUTPUT_BASE/CWE${CWE}_${TIMESTAMP}"
  FULL_DIR="$BASE_DIR/full_results"
  INDIVIDUAL_DIR="$BASE_DIR/individual"
  JSON_PATH="$FULL_DIR/CWE${CWE}_${TIMESTAMP}.json"
  if [[ -f "$JSON_PATH" ]]; then
    if command -v jq >/dev/null 2>&1; then
      echo "   jq . $JSON_PATH"
    else
      echo "   cat $JSON_PATH"
    fi
    [[ -d "$INDIVIDUAL_DIR" ]] && echo "   (Per-file JSONs in: $INDIVIDUAL_DIR/ )"
  fi
done
echo ""
echo "=========================================="
