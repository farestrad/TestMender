#!/usr/bin/env python3
"""
test_static_analysis.py

Test script to verify Semgrep, Cppcheck, and Flawfinder installations
and demonstrate their usage on a test vulnerability file.
"""

import subprocess
import sys
import os
from pathlib import Path

def run_command(cmd, description):
    """Run a command and return success status."""
    print(f"\n{'='*60}")
    print(f"Testing: {description}")
    print(f"Command: {' '.join(cmd)}")
    print('='*60)
    
    try:
        result = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            timeout=30
        )
        
        if result.stdout:
            print("STDOUT:")
            print(result.stdout)
        
        if result.stderr:
            print("STDERR:")
            print(result.stderr)
        
        if result.returncode == 0 or (description.startswith("Semgrep") and result.returncode == 1):
            # Semgrep returns 1 when findings are found, which is expected
            print(f"✅ {description} - SUCCESS")
            return True
        else:
            print(f"❌ {description} - FAILED (exit code: {result.returncode})")
            return False
            
    except FileNotFoundError:
        print(f"❌ {description} - Tool not found!")
        print(f"   Please install the tool and try again.")
        return False
    except subprocess.TimeoutExpired:
        print(f"❌ {description} - Timeout!")
        return False
    except Exception as e:
        print(f"❌ {description} - Error: {e}")
        return False

def check_tool_version(tool_name, version_cmd):
    """Check if a tool is installed and get its version."""
    print(f"\n🔍 Checking {tool_name}...")
    try:
        result = subprocess.run(
            version_cmd,
            capture_output=True,
            text=True,
            timeout=5
        )
        if result.returncode == 0:
            version_output = (result.stdout + result.stderr).strip().split('\n')[0]
            print(f"✅ {tool_name} installed: {version_output}")
            return True
        else:
            print(f"❌ {tool_name} not found")
            return False
    except FileNotFoundError:
        print(f"❌ {tool_name} not installed")
        return False
    except Exception as e:
        print(f"❌ Error checking {tool_name}: {e}")
        return False

def main():
    print("="*60)
    print("Static Analysis Tools Test Suite")
    print("="*60)
    
    # Check if test file exists
    test_file = Path("test_vulnerabilities.c")
    if not test_file.exists():
        print(f"❌ Test file not found: {test_file}")
        print("   Please run this script from the project root directory.")
        sys.exit(1)
    
    print(f"✅ Test file found: {test_file}")
    
    # Check tool versions
    print("\n" + "="*60)
    print("Checking Tool Installations")
    print("="*60)
    
    semgrep_ok = check_tool_version("Semgrep", ["semgrep", "--version"])
    cppcheck_ok = check_tool_version("Cppcheck", ["cppcheck", "--version"])
    flawfinder_ok = check_tool_version("Flawfinder", ["flawfinder", "--version"])
    
    if not (semgrep_ok and cppcheck_ok and flawfinder_ok):
        print("\n⚠️  Some tools are missing. Please install them:")
        if not semgrep_ok:
            print("   - Semgrep: pip install semgrep")
        if not cppcheck_ok:
            print("   - Cppcheck: brew install cppcheck (macOS) or sudo apt install cppcheck (Ubuntu)")
        if not flawfinder_ok:
            print("   - Flawfinder: pip install flawfinder")
        sys.exit(1)
    
    # Run tests
    print("\n" + "="*60)
    print("Running Static Analysis Tests")
    print("="*60)
    
    results = []
    
    # Test 1: Semgrep with auto config
    results.append(run_command(
        ["semgrep", "--config=auto", str(test_file)],
        "Semgrep (auto config)"
    ))
    
    # Test 2: Semgrep with C/C++ security rules
    results.append(run_command(
        ["semgrep", "--config=p/c-cpp-security", str(test_file)],
        "Semgrep (C/C++ security rules)"
    ))
    
    # Test 3: Cppcheck basic
    results.append(run_command(
        ["cppcheck", str(test_file)],
        "Cppcheck (basic scan)"
    ))
    
    # Test 4: Cppcheck with all checks
    results.append(run_command(
        ["cppcheck", "--enable=all", "--suppress=missingIncludeSystem", str(test_file)],
        "Cppcheck (all checks enabled)"
    ))
    
    # Test 5: Flawfinder default
    results.append(run_command(
        ["flawfinder", str(test_file)],
        "Flawfinder (default scan)"
    ))
    
    # Test 6: Flawfinder with high risk only
    results.append(run_command(
        ["flawfinder", "--minlevel=3", str(test_file)],
        "Flawfinder (high risk only)"
    ))
    
    # Summary
    print("\n" + "="*60)
    print("Test Summary")
    print("="*60)
    
    passed = sum(results)
    total = len(results)
    
    print(f"Passed: {passed}/{total}")
    
    if passed == total:
        print("✅ All tests passed!")
        print("\n🎉 All static analysis tools are working correctly!")
        print("\nYou can now use these tools on Juliet test cases:")
        print("  - semgrep --config=p/c-cpp-security data/juliet/testcases/CWE121/")
        print("  - cppcheck --enable=all data/juliet/testcases/CWE121/")
        print("  - flawfinder --minlevel=1 data/juliet/testcases/CWE121/")
        return 0
    else:
        print(f"❌ {total - passed} test(s) failed")
        return 1

if __name__ == "__main__":
    sys.exit(main())