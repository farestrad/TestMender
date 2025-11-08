#!/usr/bin/env bash
# =========================================================
# analyze_cwe.sh — Run all static analysis tools on a CWE
# Usage: ./scripts/analyze_cwe.sh <CWE_NUMBER>
# Example: ./scripts/analyze_cwe.sh 121
# =========================================================
set -euo pipefail

CWE="${1:-}"
if [[ -z "$CWE" ]]; then
  echo "Usage: $0 <CWE_NUMBER>"
  echo "Example: $0 121"
  exit 1
fi

SOURCE_DIR="data/juliet/testcases/CWE${CWE}"
OUTPUT_DIR="analysis_results/CWE${CWE}"

if [[ ! -d "$SOURCE_DIR" ]]; then
  echo "❌ CWE${CWE} source directory not found: $SOURCE_DIR"
  exit 1
fi

# Create output directory
mkdir -p "$OUTPUT_DIR"

echo "="
echo "🔍 Analyzing CWE${CWE} with static analysis tools"
echo "   Source: $SOURCE_DIR"
echo "   Output: $OUTPUT_DIR"
echo "="

# Run Semgrep
echo ""
echo "📊 Running Semgrep..."
semgrep --config=p/c-cpp-security \
  --json \
  --output="$OUTPUT_DIR/semgrep_results.json" \
  "$SOURCE_DIR" || true

semgrep --config=p/c-cpp-security \
  --output="$OUTPUT_DIR/semgrep_results.txt" \
  "$SOURCE_DIR" || true

echo "   ✅ Semgrep results saved to:"
echo "      - $OUTPUT_DIR/semgrep_results.json"
echo "      - $OUTPUT_DIR/semgrep_results.txt"

# Run Cppcheck
echo ""
echo "📊 Running Cppcheck..."
cppcheck --enable=all \
  --xml \
  --xml-version=2 \
  --suppress=missingIncludeSystem \
  "$SOURCE_DIR" 2>"$OUTPUT_DIR/cppcheck_results.xml" || true

cppcheck --enable=all \
  --suppress=missingIncludeSystem \
  "$SOURCE_DIR" 2>"$OUTPUT_DIR/cppcheck_results.txt" || true

echo "   ✅ Cppcheck results saved to:"
echo "      - $OUTPUT_DIR/cppcheck_results.xml"
echo "      - $OUTPUT_DIR/cppcheck_results.txt"

# Run Flawfinder
echo ""
echo "📊 Running Flawfinder..."
flawfinder --minlevel=1 \
  --csv \
  "$SOURCE_DIR" > "$OUTPUT_DIR/flawfinder_results.csv" || true

flawfinder --minlevel=1 \
  --context \
  "$SOURCE_DIR" > "$OUTPUT_DIR/flawfinder_results.txt" || true

flawfinder --minlevel=1 \
  --html \
  "$SOURCE_DIR" > "$OUTPUT_DIR/flawfinder_results.html" || true

echo "   ✅ Flawfinder results saved to:"
echo "      - $OUTPUT_DIR/flawfinder_results.csv"
echo "      - $OUTPUT_DIR/flawfinder_results.txt"
echo "      - $OUTPUT_DIR/flawfinder_results.html"

# Summary
echo ""
echo "="
echo "✅ Analysis complete for CWE${CWE}"
echo "   Results directory: $OUTPUT_DIR"
echo ""
echo "📄 View results:"
echo "   cat $OUTPUT_DIR/semgrep_results.txt"
echo "   cat $OUTPUT_DIR/cppcheck_results.txt"
echo "   cat $OUTPUT_DIR/flawfinder_results.txt"
echo "   open $OUTPUT_DIR/flawfinder_results.html"
echo "="