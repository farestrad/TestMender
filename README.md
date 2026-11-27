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

### 6. Fetch and rebuild the pinned Juliet Test Suite

Requires Git, Python 3, CMake, Make and a C/C++ compiler (see system dependencies above).
A normal Git clone or GitHub ZIP download works; the setup command downloads the
source revision pinned by `scripts/rebuild_juliet.py` from upstream over HTTPS.
No API keys are needed for this build.

```bash
bash scripts/setup_juliet.sh
# Small compile-only check first:
python3 scripts/rebuild_juliet.py 546 --jobs 2
# Select the categories needed for your work:
python3 scripts/rebuild_juliet.py 121 122 --jobs 2
# Or attempt every category (can take substantial time and disk space):
python3 scripts/rebuild_juliet.py --all --jobs 2
```

Commands resolve paths relative to the project, so they also work from another
working directory when invoked by absolute path. An existing dataset must match
the pinned revision and have no tracked edits; setup never pulls, resets or deletes
existing files. If it reports a mismatch, preserve the existing dataset and build
in a fresh project download. Recursive Git clones also use the corrected pinned
Juliet submodule entry.

Binaries are written to `data/juliet/bin/CWE*/good/` and `bad/`; intermediate
build files go under `build/juliet/`. Category `CMakeLists.txt` files are copied
from Juliet's tracked template. These generated files do not need to be published.
The source pin matches the local dataset used to produce the original artifacts;
compiler/OS differences mean binaries are not guaranteed to be byte-identical.

Juliet deliberately contains vulnerable programs. The new setup/build commands
only fetch and compile; they never run those programs or require elevated rights.
Use an isolated disposable environment for execution. The legacy
`scripts/verify_setup.sh` and upstream `juliet.py -r` **execute test programs**;
they are not required to verify a build. Build without credentials in the
environment when practical. Do not use `sudo` for the build.

A category may fail on a different platform or compiler. The builder continues
through selected categories, lists failures and exits nonzero if any fail.
A successful category build confirms compilation, not vulnerability detection.

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

Use `python3 scripts/rebuild_juliet.py <CWE numbers> --jobs 2` or `--all`.
The wrapper verifies the pinned source, configures CMake and compiles with bounded
parallelism. It reports build failures with a nonzero exit status.

## 🚨 Troubleshooting

### Build Failures

Check the compiler output. Some categories require platform-specific APIs or
additional libraries; do not assume all categories build on every OS. Retry a
specific category after resolving its dependencies. Existing build outputs are
left in place for inspection.

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


## 🔑 LLM API Setup (Gemini or GPT)

TestMender supports **two providers** for automatic test case generation:
- **Google Gemini 2.5 Flash**
- **OpenAI GPT-4o-mini**

You can freely switch between them using the `--provider` flag.

---

### 🧩 1. Create and activate a Python virtual environment

Use Python 3.11 for compatibility:


/Library/Frameworks/Python.framework/Versions/3.11/bin/python3 -m venv .venv
source .venv/bin/activate
python -V  # should show Python 3.11.x
⚙️ 2. Install dependencies
bash
Copy code
python -m pip install -U pip
python -m pip install python-dotenv google-generativeai openai
🔐 3. Add your API keys
Create a .env file in the project root and include one or both of the following:


# For Gemini
GEMINI_API_KEY=your_gemini_api_key_here

# For OpenAI
OPENAI_API_KEY=your_openai_api_key_here
Only the key matching your chosen provider will be used.

🧠 4. Generate test cases
Run from the project root:


# Using Gemini 2.5 Flash
python scripts/generate_test_cases_llm.py --cwe 121 --provider gemini --model gemini-2.5-flash

# Using GPT-4o-mini
python scripts/generate_test_cases_llm.py --cwe 121 --provider openai --model gpt-4o-mini
✅ This will:

Randomly select up to 20 Juliet .c files for the given CWE

Ask the LLM to generate 5 test cases per file

Save results to generated_test_cases/CWE<id>/

🧪 5. Run a generated harness (compile + execute)
Example for CWE-121:


./scripts/run_harness.sh 121 CWE121_Stack_Based_Buffer_Overflow__char_type_overrun_memcpy_09_harness.c
This compiles with AddressSanitizer, automatically links the correct Juliet sources, and runs the binary.