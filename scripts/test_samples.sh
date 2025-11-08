#!/usr/bin/env bash
# =========================================================
# test_samples.sh — Test all static analysis tools on sample files
# Usage: ./scripts/test_samples.sh
# =========================================================
set -euo pipefail

echo "="
echo "🔍 Testing Static Analysis Tools on Sample Files"
echo "="

# Array of test files
TEST_FILES=(
    "test_vulnerabilities.c"
    "simple_buffer_overflow.c"
    "format_string_vuln.c"
    "use_after_free.c"
)

# Create output directory
OUTPUT_DIR="sample_analysis_results"
mkdir -p "$OUTPUT_DIR"

# Function to test a file with all tools
test_file() {
    local file="$1"
    local basename=$(basename "$file" .c)
    
    echo ""
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    echo "📄 Analyzing: $file"
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    
    if [[ ! -f "$file" ]]; then
        echo "❌ File not found: $file"
        return 1
    fi
    
    # Semgrep
    echo ""
    echo "🔹 Semgrep Results:"
    semgrep --config=auto "$file" 2>&1 | tee "$OUTPUT_DIR/${basename}_semgrep.txt" || true
    
    # Cppcheck
    echo ""
    echo "🔹 Cppcheck Results:"
    cppcheck --enable=all --suppress=missingIncludeSystem "$file" 2>&1 | tee "$OUTPUT_DIR/${basename}_cppcheck.txt" || true
    
    # Flawfinder
    echo ""
    echo "🔹 Flawfinder Results:"
    flawfinder --minlevel=1 "$file" 2>&1 | tee "$OUTPUT_DIR/${basename}_flawfinder.txt" || true
    
    echo ""
    echo "✅ Results saved to $OUTPUT_DIR/${basename}_*.txt"
}

# Check if files exist
missing_files=0
for file in "${TEST_FILES[@]}"; do
    if [[ ! -f "$file" ]]; then
        echo "⚠️  Warning: $file not found"
        missing_files=$((missing_files + 1))
    fi
done

if [[ $missing_files -gt 0 ]]; then
    echo ""
    echo "❌ $missing_files test file(s) missing"
    echo "   Please ensure you're running from the project root directory"
    exit 1
fi

# Test each file
for file in "${TEST_FILES[@]}"; do
    test_file "$file"
done

# Summary
echo ""
echo "="
echo "✅ Analysis Complete!"
echo "="
echo ""
echo "📊 Summary of findings:"
echo ""

for file in "${TEST_FILES[@]}"; do
    basename=$(basename "$file" .c)
    echo "📄 $file:"
    
    # Count findings
    semgrep_count=$(grep -c "^\s*[0-9]\+┆" "$OUTPUT_DIR/${basename}_semgrep.txt" 2>/dev/null || echo "0")
    cppcheck_count=$(grep -c "\[.*\]:" "$OUTPUT_DIR/${basename}_cppcheck.txt" 2>/dev/null || echo "0")
    flawfinder_count=$(grep -c "^$file:" "$OUTPUT_DIR/${basename}_flawfinder.txt" 2>/dev/null || echo "0")
    
    echo "   Semgrep:    $semgrep_count issue(s)"
    echo "   Cppcheck:   $cppcheck_count issue(s)"
    echo "   Flawfinder: $flawfinder_count issue(s)"
    echo ""
done

echo "📁 Detailed results saved in: $OUTPUT_DIR/"
echo ""
echo "🔍 To view detailed results:"
echo "   cat $OUTPUT_DIR/test_vulnerabilities_semgrep.txt"
echo "   cat $OUTPUT_DIR/simple_buffer_overflow_cppcheck.txt"
echo "   cat $OUTPUT_DIR/format_string_vuln_flawfinder.txt"