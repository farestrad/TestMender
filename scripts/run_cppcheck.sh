#!/usr/bin/env bash
# =========================================================
# run_cppcheck.sh — Run Cppcheck on specified CWE(s)
# Usage: ./scripts/run_cppcheck.sh <CWE_NUMBER> [CWE_NUMBER...]
# Example: ./scripts/run_cppcheck.sh 121 122 190
# =========================================================
set -euo pipefail

# Check if at least one CWE is provided
if [[ $# -eq 0 ]]; then
  echo "Usage: $0 <CWE_NUMBER> [CWE_NUMBER...]"
  echo "Example: $0 121 122 190"
  exit 1
fi

OUTPUT_BASE="analysis_results/cppcheck"
mkdir -p "$OUTPUT_BASE"

# Timestamp for this run
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")

echo "=========================================="
echo "🔍 Running Cppcheck Analysis"
echo "=========================================="
echo "CWEs to analyze: $*"
echo "Output directory: $OUTPUT_BASE"
echo "=========================================="

# Process each CWE
for CWE in "$@"; do
    SOURCE_DIR="data/juliet/testcases/CWE${CWE}"
    
    if [[ ! -d "$SOURCE_DIR" ]]; then
        echo "⚠️  Warning: CWE${CWE} source directory not found: $SOURCE_DIR"
        echo "   Skipping..."
        continue
    fi
    
    XML_OUTPUT="$OUTPUT_BASE/CWE${CWE}_${TIMESTAMP}.xml"
    JSON_OUTPUT="$OUTPUT_BASE/CWE${CWE}_${TIMESTAMP}.json"
    
    echo ""
    echo "📊 Analyzing CWE${CWE}..."
    echo "   Source: $SOURCE_DIR"
    echo "   Output: $JSON_OUTPUT"
    
    # Run Cppcheck with XML output first
    cppcheck --enable=all \
        --xml \
        --xml-version=2 \
        --suppress=missingIncludeSystem \
        --quiet \
        "$SOURCE_DIR" 2>"$XML_OUTPUT" || {
            echo "   ⚠️  Cppcheck completed with warnings (this is normal)"
        }
    
    # Convert XML to JSON using Python
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
    JSON_OUTPUT="$OUTPUT_BASE/CWE${CWE}_${TIMESTAMP}.json"
    if [[ -f "$JSON_OUTPUT" ]]; then
        echo "   jq . $JSON_OUTPUT"
    fi
done
echo ""
echo "=========================================="
