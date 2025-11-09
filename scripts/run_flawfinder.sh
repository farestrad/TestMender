#!/usr/bin/env bash
# =========================================================
# run_flawfinder.sh — Run Flawfinder on specified CWE(s)
# Usage: ./scripts/run_flawfinder.sh <CWE_NUMBER> [CWE_NUMBER...]
# Example: ./scripts/run_flawfinder.sh 121 122 190
# =========================================================
set -euo pipefail

# Check if at least one CWE is provided
if [[ $# -eq 0 ]]; then
  echo "Usage: $0 <CWE_NUMBER> [CWE_NUMBER...]"
  echo "Example: $0 121 122 190"
  exit 1
fi

OUTPUT_BASE="analysis_results/flawfinder"
mkdir -p "$OUTPUT_BASE"

# Timestamp for this run
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")

echo "=========================================="
echo "🔍 Running Flawfinder Analysis"
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
    
    CSV_OUTPUT="$OUTPUT_BASE/CWE${CWE}_${TIMESTAMP}.csv"
    JSON_OUTPUT="$OUTPUT_BASE/CWE${CWE}_${TIMESTAMP}.json"
    
    echo ""
    echo "📊 Analyzing CWE${CWE}..."
    echo "   Source: $SOURCE_DIR"
    echo "   Output: $JSON_OUTPUT"
    
    # Run Flawfinder with CSV output first
    flawfinder --minlevel=1 \
        --csv \
        --quiet \
        "$SOURCE_DIR" > "$CSV_OUTPUT" 2>/dev/null || {
            echo "   ⚠️  Flawfinder completed with warnings (this is normal)"
        }
    
    # Convert CSV to JSON using Python
    python3 - <<EOF
import csv
import json
import sys

try:
    results = {
        'tool': 'flawfinder',
        'cwe': '$CWE',
        'source_dir': '$SOURCE_DIR',
        'timestamp': '$TIMESTAMP',
        'findings': []
    }
    
    with open('$CSV_OUTPUT', 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            # Flawfinder CSV columns: File,Line,Column,Level,Category,Name,Warning,Suggestion,Note,CWEs
            finding = {
                'file': row.get('File', ''),
                'line': row.get('Line', ''),
                'column': row.get('Column', ''),
                'level': row.get('Level', ''),
                'category': row.get('Category', ''),
                'name': row.get('Name', ''),
                'warning': row.get('Warning', ''),
                'suggestion': row.get('Suggestion', ''),
                'note': row.get('Note', ''),
                'cwes': row.get('CWEs', '')
            }
            results['findings'].append(finding)
    
    with open('$JSON_OUTPUT', 'w') as f:
        json.dump(results, f, indent=2)
    
    print(f"   ✅ Analysis complete")
    print(f"   📈 Found {len(results['findings'])} issue(s)")
    
except Exception as e:
    print(f"   ❌ Error converting CSV to JSON: {e}", file=sys.stderr)
    sys.exit(1)
EOF

done

echo ""
echo "=========================================="
echo "✅ Flawfinder Analysis Complete"
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
