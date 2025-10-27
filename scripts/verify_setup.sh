#!/usr/bin/env bash
set -euo pipefail

# Store the original directory
ORIGINAL_DIR=$(pwd)

echo "🔍 Verifying TestMender setup..."

# Check if Juliet dataset exists
if [ ! -d "data/juliet" ]; then
    echo "❌ Juliet dataset not found. Run: scripts/setup_juliet.sh"
    exit 1
fi

# Check if Juliet is built
if [ ! -d "data/juliet/bin/CWE121" ]; then
    echo "❌ Juliet not built. Run: cd data/juliet && python3 juliet.py -a -g -m -k"
    exit 1
fi

# Test good executable
echo "🧪 Testing good executable..."
cd data/juliet/bin/CWE121/good
if echo "5" | ./CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_01-good > /dev/null 2>&1; then
    echo "✅ Good executable works"
else
    echo "❌ Good executable failed"
    exit 1
fi

# Test bad executable (should crash)
echo "🧪 Testing bad executable..."
cd ../bad
if echo "15" | ./CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_01-bad > /dev/null 2>&1; then
    echo "❌ Bad executable should have crashed but didn't"
    exit 1
else
    echo "✅ Bad executable crashes as expected (buffer overflow)"
fi

# Count built CWEs
cd "$ORIGINAL_DIR"
BUILT_CWES=$(ls data/juliet/bin/ | grep "^CWE" | wc -l)
echo "📊 Built CWE categories: $BUILT_CWES"

echo "🎉 Setup verification complete!"
echo "   Ready for static analysis and defect detection research."