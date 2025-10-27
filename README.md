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


### 4. Install system dependencies

**macOS (Homebrew):**
```bash
brew install cmake make gcc g++
```

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install cmake make gcc g++ build-essential
```

**CentOS/RHEL:**
```bash
sudo yum install cmake make gcc gcc-c++
```

### 5. Setup Juliet Test Suite
```bash
# Clone the Juliet dataset (git-ignored)
scripts/setup_juliet.sh

# Build test cases (this will take several minutes)
cd data/juliet
python3 juliet.py -a -g -m -k
```

### 6. Verify setup
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
