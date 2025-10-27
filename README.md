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

### 4. Setup Juliet Test Suite
```bash
# Clone the Juliet dataset (git-ignored)
scripts/setup_juliet.sh

# Build test cases (this will take several minutes)
cd data/juliet
python3 juliet.py -a -g -m -k
```

### 5. Verify setup
```bash
# Run automated verification
scripts/verify_setup.sh
```

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