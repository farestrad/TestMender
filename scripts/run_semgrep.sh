#!/usr/bin/env bash
# =========================================================
# run_semgrep.sh — Run Semgrep on specified CWE(s)
# Usage: ./scripts/run_semgrep.sh <CWE_NUMBER> [CWE_NUMBER...]
# Example: ./scripts/run_semgrep.sh 121 122 190
# =========================================================
set -euo pipefail

# --- Guards ------------------------------------------------
if ! command -v semgrep >/dev/null 2>&1; then
  echo "❌ Semgrep is not installed. Install via Homebrew (brew install semgrep) or pipx (pipx install semgrep)."
  exit 1
fi

if [[ $# -eq 0 ]]; then
  echo "Usage: $0 <CWE_NUMBER> [CWE_NUMBER...]"
  echo "Example: $0 121 122 190"
  exit 1
fi

# --- Paths & Output ---------------------------------------
OUTPUT_BASE="analysis_results/semgrep"
mkdir -p "$OUTPUT_BASE"

TIMESTAMP=$(date +"%Y%m%d_%H%M%S")

# Prefer vendored rules (submodule) → deterministic & offline
LOCAL_RULES_DIR="tools/semgrep-rules/c/lang/security"

# Registry fallbacks (only if vendored path missing)
REGISTRY_FALLBACKS=(
  "p/c"               # C/C++ ruleset (may include Pro rules)
  "p/security-audit"  # broader security audit
  "p/default"         # general defaults
  "auto"              # let semgrep choose (contacts registry)
)

echo "=========================================="
echo "🔍 Running Semgrep Analysis"
echo "=========================================="
echo "CWEs to analyze: $*"
echo "Output directory: $OUTPUT_BASE"
echo "=========================================="

# --- Choose configuration ---------------------------------
WORKING_CONFIG=""
if [[ -d "$LOCAL_RULES_DIR" ]]; then
  WORKING_CONFIG="$LOCAL_RULES_DIR"
  echo "🔧 Using vendored rules: $WORKING_CONFIG"
else
  echo "🔧 Vendored rules not found; trying registry packs..."
  for cfg in "${REGISTRY_FALLBACKS[@]}"; do
    if semgrep scan --config="$cfg" --dryrun >/dev/null 2>&1; then
      WORKING_CONFIG="$cfg"
      echo "   ✅ Will use registry pack: $cfg"
      break
    fi
  done
fi

if [[ -z "$WORKING_CONFIG" ]]; then
  echo "❌ No working Semgrep configuration found (vendored rules missing and registry not reachable)."
  echo "   Make sure submodule is initialized:  git submodule update --init --recursive"
  exit 1
fi

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

  OUTPUT_FILE="$OUTPUT_BASE/CWE${CWE}_${TIMESTAMP}.json"
  OUTPUT_DIR="$OUTPUT_BASE/CWE${CWE}_${TIMESTAMP}"
  mkdir -p "$OUTPUT_DIR"

  echo ""
  echo "📊 Analyzing CWE${CWE}..."
  echo "   Source: $SOURCE_DIR"
  echo "   Config: $WORKING_CONFIG"
  echo "   Output (pretty JSON): $OUTPUT_FILE"

  # --- Semgrep run (into temp), then pretty-print ----------
  TEMP_JSON="$(mktemp)"
  if semgrep scan \
      --config="$WORKING_CONFIG" \
      --json --output="$TEMP_JSON" \
      --metrics=off --error \
      "$SOURCE_DIR" >/dev/null 2>&1; then
    :
  else
    # If it failed, but output exists, keep going (often still has results)
    if [[ ! -s "$TEMP_JSON" ]]; then
      echo "   ❌ Semgrep failed and produced no output"
      rm -f "$TEMP_JSON"
      continue
    else
      echo "   ⚠️  Semgrep returned non-zero (often normal if some rules error); output captured."
    fi
  fi

  # Pretty-print JSON
  if command -v jq >/dev/null 2>&1; then
    jq '.' "$TEMP_JSON" > "$OUTPUT_FILE"
  else
    cp "$TEMP_JSON" "$OUTPUT_FILE"
  fi
  rm -f "$TEMP_JSON"

  # --- Summaries & per-file breakdown ----------------------
  if [[ -f "$OUTPUT_FILE" ]]; then
    echo "   ✅ Analysis complete (results formatted)"
    if command -v jq >/dev/null 2>&1; then
      FINDING_COUNT=$(jq '.results | length' "$OUTPUT_FILE" 2>/dev/null || echo "0")
      ERROR_COUNT=$(jq '.errors  | length' "$OUTPUT_FILE" 2>/dev/null || echo "0")
      echo "   📈 Found $FINDING_COUNT issue(s)"
      [[ "$ERROR_COUNT" -gt 0 ]] && echo "   ⚠️  $ERROR_COUNT error(s) reported in Semgrep JSON"

      echo "------------------------------------------"
      echo "📂 Per-file summary:"
      jq -r '
        .results[]
        | {file:.path, cwe:(.extra.metadata.cwe[0]? // "unknown CWE")}
        | "\(.file) → \(.cwe)"
      ' "$OUTPUT_FILE" | sort | uniq

      echo "------------------------------------------"
      echo "📊 Top files by finding count:"
      TSV_TABLE=$(
        jq -r '
          .results
          | group_by(.path)
          | map({path: .[0].path,
                 n: length,
                 rules: ([.[].check_id] | unique | length),
                 cwes: ([.[].extra.metadata.cwe[]?] | unique | join("; "))})
          | sort_by(-.n)[:20]
          | (["count","rules","path","cwes"]),
            (.[] | [(.n|tostring), (.rules|tostring), .path, .cwes])
          | @tsv
        ' "$OUTPUT_FILE"
      )
      if command -v column >/dev/null 2>&1; then
        # shellcheck disable=SC2001
        echo "$TSV_TABLE" | column -t
      else
        echo "$TSV_TABLE"
      fi

      # CSV summary next to the JSON
      CSV_FILE="${OUTPUT_FILE%.json}.summary.csv"
      jq -r '
        .results
        | group_by(.path)
        | map({path: .[0].path,
               count: length,
               rules: ([.[].check_id] | unique | join(";")),
               cwes: ([.[].extra.metadata.cwe[]?] | unique | join(";"))})
        | (["path","count","rules","cwes"]),
          (.[] | [ .path, ( .count|tostring ), .rules, .cwes ])
        | @csv
      ' "$OUTPUT_FILE" > "$CSV_FILE"
      echo "🧾 Wrote CSV summary: $CSV_FILE"

      echo "------------------------------------------"
      echo "🧪 Function/file hint (GOOD vs BAD):"
      if command -v rg >/dev/null 2>&1; then
        # Smarter: filename OR content shows function bad()/good()
        while IFS= read -r f; do
          if [[ "$f" =~ [Gg][Oo][Oo][Dd] ]] || rg -n --no-mmap --fixed-strings "void good" "$f" >/dev/null 2>&1; then
            echo "✅ GOOD: $f"
          elif [[ "$f" =~ [Bb][Aa][Dd] ]] || rg -n --no-mmap --fixed-strings "void bad" "$f" >/dev/null 2>&1; then
            echo "❌ BAD:  $f"
          else
            echo "⚙️  MIXED/UNKNOWN: $f"
          fi
        done < <(jq -r '.results[].path' "$OUTPUT_FILE" | sort -u)
      else
        # Fallback: filename heuristic only
        jq -r '
          .results[]
          | .path as $file
          | if ($file | test("good"; "i")) then
              "✅ GOOD: " + $file
            elif ($file | test("bad"; "i")) then
              "❌ BAD:  " + $file
            else
              "⚙️  MIXED/UNKNOWN: " + $file
            end
        ' "$OUTPUT_FILE" | sort -u
      fi

      echo "------------------------------------------"
      echo "🗂  Writing per-file JSONs to: $OUTPUT_DIR/"
      # Split results by file into separate pretty JSONs
      # Each file: { version, path, results:[...only for that path...] }
      mapfile -t FILES < <(jq -r '.results[].path' "$OUTPUT_FILE" | sort -u)
      for f in "${FILES[@]}"; do
        safe_name="$(echo "$f" | tr '/ ' '__')"
        per_file="$OUTPUT_DIR/${safe_name}.json"
        jq --arg p "$f" '
          {
            version: .version,
            path: $p,
            results: [ .results[] | select(.path == $p) ]
          }
        ' "$OUTPUT_FILE" > "$per_file"
        echo "   • $(basename "$per_file")"
      done
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
echo "✅ Semgrep Analysis Complete"
echo "=========================================="
echo ""
echo "📁 Results saved in: $OUTPUT_BASE/"
echo ""
echo "🔍 View results:"
for CWE in "$@"; do
  OUTPUT_FILE="$OUTPUT_BASE/CWE${CWE}_${TIMESTAMP}.json"
  if [[ -f "$OUTPUT_FILE" ]]; then
    if command -v jq >/dev/null 2>&1; then
      echo "   jq . $OUTPUT_FILE"
    else
      echo "   cat $OUTPUT_FILE"
    fi
    SUBDIR="$OUTPUT_BASE/CWE${CWE}_${TIMESTAMP}"
    [[ -d "$SUBDIR" ]] && echo "   (Per-file JSONs in: $SUBDIR/ )"
  fi
done
echo ""
echo "=========================================="
