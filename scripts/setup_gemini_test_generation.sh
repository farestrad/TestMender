#!/usr/bin/env bash
# =========================================================
# setup_gemini_test_generation.sh
# Quick setup script for Gemini test case generation
# =========================================================
set -euo pipefail

echo "="
echo "🚀 Setting up Gemini Test Case Generation"
echo "="

# Check if running from project root
if [[ ! -f "requirements.txt" ]]; then
    echo "❌ Please run this script from the project root directory"
    exit 1
fi

# Check Python version
echo ""
echo "🐍 Checking Python version..."
if ! command -v python3 &> /dev/null; then
    echo "❌ Python 3 is not installed"
    exit 1
fi

PYTHON_VERSION=$(python3 --version | cut -d' ' -f2 | cut -d'.' -f1,2)
echo "✅ Python $PYTHON_VERSION found"

# Check if virtual environment exists
if [[ ! -d ".venv" ]]; then
    echo ""
    echo "📦 Creating virtual environment..."
    python3 -m venv .venv
    echo "✅ Virtual environment created"
fi

# Activate virtual environment
echo ""
echo "🔧 Activating virtual environment..."
source .venv/bin/activate

# Install/upgrade required packages
echo ""
echo "📦 Installing required packages..."
pip install --upgrade pip > /dev/null
pip install google-generativeai python-dotenv > /dev/null
echo "✅ Packages installed"

# Check if .env file exists
if [[ ! -f ".env" ]]; then
    echo ""
    echo "⚙️  Creating .env file..."
    cat > .env << 'EOF'
# Gemini API Configuration
GEMINI_API_KEY=your-api-key-here

# Optional: Juliet dataset location (default: data/juliet)
# JULIET_DIR=data/juliet
EOF
    echo "✅ .env file created"
    echo ""
    echo "⚠️  IMPORTANT: Edit .env and add your Gemini API key!"
    echo "   Get your key from: https://makersuite.google.com/app/apikey"
else
    echo ""
    echo "✅ .env file already exists"
fi

# Check if Gemini API key is set
source .env 2>/dev/null || true
if [[ -z "${GEMINI_API_KEY:-}" ]] || [[ "$GEMINI_API_KEY" == "your-api-key-here" ]]; then
    echo ""
    echo "⚠️  Gemini API key not set in .env file"
    echo "   Please edit .env and add your API key"
    echo "   Get your key from: https://makersuite.google.com/app/apikey"
else
    echo ""
    echo "✅ Gemini API key is set"
fi

# Check if Juliet dataset exists
if [[ ! -d "data/juliet/testcases" ]]; then
    echo ""
    echo "⚠️  Juliet dataset not found"
    echo "   Run: ./scripts/setup_juliet.sh"
    echo "   Then: cd data/juliet && python3 juliet.py -a -g -m -k"
else
    echo ""
    echo "✅ Juliet dataset found"
    
    # Count available CWE directories
    CWE_COUNT=$(find data/juliet/testcases -maxdepth 1 -type d -name "CWE*" | wc -l)
    echo "   Found $CWE_COUNT CWE directories"
fi

# Make scripts executable
echo ""
echo "🔧 Making scripts executable..."
chmod +x generate_test_cases_gemini.py 2>/dev/null || true
chmod +x analyze_generated_tests.py 2>/dev/null || true
echo "✅ Scripts are executable"

# Final summary
echo ""
echo "="
echo "✅ Setup complete!"
echo "="
echo ""
echo "📋 Next steps:"
echo ""
echo "1. Set your Gemini API key in .env:"
echo "   nano .env  # or use your favorite editor"
echo ""
echo "2. Make sure Juliet dataset is set up (if not already):"
echo "   ./scripts/setup_juliet.sh"
echo "   cd data/juliet && python3 juliet.py -a -g -m -k && cd ../.."
echo ""
echo "3. Run the test case generator:"
echo "   python3 generate_test_cases_gemini.py"
echo ""
echo "4. Analyze the results:"
echo "   python3 analyze_generated_tests.py"
echo ""
echo "📚 For more information, see GEMINI_TEST_GENERATION.md"
echo ""