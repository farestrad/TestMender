## 🧩 Setup Instructions

### 1. Create and activate a virtual environment
```bash
python3 -m venv .venv
source .venv/bin/activate
```

### 2. Install Python dependencies
```bash
pip install -r requirements.txt
```
### 3. Install system dependencies

**macOS (requires Homebrew):**
If you don't already have Homebrew, install it first:
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
echo 'eval "$(/opt/homebrew/bin/brew shellenv)"' >> ~/.zprofile
eval "$(/opt/homebrew/bin/brew shellenv)"
```

### 4. Install system dependencies

**macOS (Homebrew):**
```bash
brew install cmake make gcc g++ cppcheck
```

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install cmake make gcc g++ build-essential cppcheck
```

**CentOS/RHEL:**
```bash
sudo yum install cmake make gcc gcc-c++ cppcheck
```

### 5. Install Static Analysis Tools

The following static analysis tools are used for automated vulnerability detection:

#### Semgrep
Semgrep is installed via pip (included in requirements.txt). Verify installation:
```bash
semgrep --version
```

#### Cppcheck
Already installed in step 4. Verify installation:
```bash
cppcheck --version
```

#### Flawfinder
Flawfinder is installed via pip (included in requirements.txt). Verify installation:
```bash
flawfinder --version
```

### 6. Setup Juliet Test Suite
```bash
# Clone the Juliet dataset (git-ignored)
scripts/setup_juliet.sh

# Build test cases (this will take several minutes)
cd data/juliet
python3 juliet.py -a -g -m -k
```

### 7. Verify setup
```bash
# Run automated verification (from project root directory)
scripts/verify_setup.sh
```

**Important:** Always run the verification script from the project root directory, not from inside the `scripts/` folder.

**Manual verification** (if needed):
```bash
# Test that executables work
cd data/juliet/bin/CWE121/good
echo "5" | ./CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_01-good

cd ../bad  
echo "15" | ./CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_01-bad
# Should crash with exit code 134 (buffer overflow)
```

## 🔍 Static Analysis Tools

### Quick Test All Tools
Run the test script to verify all static analysis tools are working:
```bash
# From project root
python3 scripts/test_static_analysis.py
```

### Individual Tool Usage

#### 1. Semgrep
Semgrep uses pattern matching and rules to find security vulnerabilities:
```bash
# Scan a single file
semgrep --config=auto path/to/file.c

# Scan a directory with security rules
semgrep --config=p/security-audit data/juliet/testcases/CWE121/

# Use specific rule sets
semgrep --config=p/c-cpp-security data/juliet/testcases/

# Output to JSON for parsing
semgrep --config=auto --json -o results.json path/to/code/
```

**Example output:**
```
CWE121_Stack_Based_Buffer_Overflow.c
  ⚠️  Use of gets() is dangerous
     15┆ gets(buffer);
```

#### 2. Cppcheck
Cppcheck focuses on detecting bugs and undefined behavior:
```bash
# Basic scan
cppcheck path/to/file.c

# Enable all checks with verbose output
cppcheck --enable=all --verbose path/to/file.c

# Check specific CWE directory
cppcheck --enable=warning,style,performance,portability data/juliet/testcases/CWE121/

# XML output for parsing
cppcheck --enable=all --xml --xml-version=2 path/to/code/ 2> cppcheck_results.xml
```

**Example output:**
```
[file.c:42]: (error) Buffer is accessed out of bounds: buffer
[file.c:15]: (warning) Obsolete function 'gets' called. It is recommended to use 'fgets' instead.
```

#### 3. Flawfinder
Flawfinder examines C/C++ code for potential security flaws:
```bash
# Basic scan
flawfinder path/to/file.c

# Scan with minimum risk level (0-5, default 1)
flawfinder --minlevel=3 path/to/file.c

# Detailed output with context
flawfinder --context --minlevel=2 data/juliet/testcases/CWE121/

# HTML report
flawfinder --html --minlevel=1 data/juliet/testcases/ > flawfinder_report.html

# CSV output for analysis
flawfinder --csv data/juliet/testcases/CWE121/ > results.csv
```

**Example output:**
```
file.c:15: [5] (buffer) gets: Does not check for buffer overflows (CWE-120, CWE-20)
file.c:42: [2] (buffer) strcpy: Does not check for buffer overflows when copying to destination
```

### Running All Tools on Juliet
```bash
# Analyze a specific CWE with all tools
scripts/analyze_cwe.sh 121

# Or run manually:
cd data/juliet/testcases/CWE121
semgrep --config=p/c-cpp-security . > semgrep_results.txt
cppcheck --enable=all . 2> cppcheck_results.txt
flawfinder --minlevel=1 . > flawfinder_results.txt
```

## 📦 Juliet Dataset Details
- **Location**: `data/juliet/` (git-ignored)
- **Source**: Cloned from GitHub repository
- **Built executables**: `data/juliet/bin/CWE*/good/` and `data/juliet/bin/CWE*/bad/`
- **Source code**: `data/juliet/testcases/CWE*/s*/`

## 🔧 Build Process
The Juliet test suite is built using:
- **CMake** for build configuration
- **Make** for compilation
- **juliet.py** script for automation

**Build command**: `python3 juliet.py -a -g -m -k`
- `-a`: Build all CWE categories
- `-g`: Generate CMake files
- `-m`: Compile with Make
- `-k`: Continue on build failures

**Expected results**: ~91 CWE categories successfully built out of 118 total

## 🚨 Troubleshooting

### Build Failures
If some CWE categories fail to build:
- This is normal - ~27 categories may fail due to platform-specific issues
- The `-k` flag ensures the build continues despite failures
- Focus on the successfully built categories for your research

### Missing Dependencies
```bash
# Check if cmake is installed
cmake --version

# Check if make is installed  
make --version

# Check if gcc is installed
gcc --version

# Check static analysis tools
semgrep --version
cppcheck --version
flawfinder --version
```

### Permission Issues
```bash
# Make scripts executable
chmod +x scripts/*.sh
```

### Verification Script Issues
If you get "Juliet dataset not found" when running `scripts/verify_setup.sh`:
- Make sure you're running it from the **project root directory**
- Don't run it from inside the `scripts/` folder
- The script needs to find `data/juliet/` relative to where you run it 

### Static Analysis Tool Issues

**Semgrep not found:**
```bash
pip install --upgrade semgrep
```

**Cppcheck not found:**
- macOS: `brew install cppcheck`
- Ubuntu: `sudo apt install cppcheck`

**Flawfinder not found:**
```bash
pip install --upgrade flawfinder
```
### 3. Test samples for static analysis tools
./scripts/test_samples.sh






## 🔑 Gemini API Setup

To enable Gemini 2.5 Flash integration for automated test case generation:

### 1. Create and activate a Python virtual environment
Use your local Python 3.11 installation to ensure compatibility:


/Library/Frameworks/Python.framework/Versions/3.11/bin/python3 -m venv .venv
source .venv/bin/activate
python -V  # should show Python 3.11.x

2. Install required packages
python -m pip install -U pip
python -m pip install python-dotenv google-generativeai

### 3. Add your Gemini API key

Create a file named .env in the project root and add your key:

GEMINI_API_KEY=your_api_key_here


### 4. Test the Gemini integration

run the following from root : scripts/test_gemini.py:

Expected output:

print("Hello, world!")


✅ If you see the above, your Gemini setup is complete.