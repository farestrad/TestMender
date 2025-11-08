#!/usr/bin/env python3
"""
compare_tools.py

Compare the detection capabilities of Semgrep, Cppcheck, and Flawfinder
on the test vulnerability files.
"""

import subprocess
import sys
from pathlib import Path

def run_tool(tool_cmd, file_path):
    """Run a tool and return its output."""
    try:
        result = subprocess.run(
            tool_cmd + [str(file_path)],
            capture_output=True,
            text=True,
            timeout=30
        )
        return result.stdout + result.stderr
    except Exception as e:
        return f"Error: {e}"

def count_findings(output, tool_name):
    """Count findings in tool output."""
    if tool_name == "semgrep":
        # Count lines with findings (line numbers)
        return output.count("┆")
    elif tool_name == "cppcheck":
        # Count lines with brackets (findings)
        return output.count("[")
    elif tool_name == "flawfinder":
        # Count lines with risk levels
        return output.count("]:")
    return 0

def main():
    print("="*70)
    print("Static Analysis Tools - Detection Comparison")
    print("="*70)
    
    test_files = [
        "test_vulnerabilities.c",
        "simple_buffer_overflow.c",
        "format_string_vuln.c",
        "use_after_free.c"
    ]
    
    # Check if files exist
    for file in test_files:
        if not Path(file).exists():
            print(f"❌ Test file not found: {file}")
            print("   Please run from project root directory")
            sys.exit(1)
    
    # Tool configurations
    tools = {
        "Semgrep": ["semgrep", "--config=auto", "--quiet"],
        "Cppcheck": ["cppcheck", "--enable=all", "--suppress=missingIncludeSystem"],
        "Flawfinder": ["flawfinder", "--minlevel=1"]
    }
    
    results = {}
    
    # Run each tool on each file
    for file in test_files:
        print(f"\n{'='*70}")
        print(f"📄 Testing: {file}")
        print('='*70)
        
        results[file] = {}
        
        for tool_name, tool_cmd in tools.items():
            print(f"\n🔹 {tool_name}:")
            output = run_tool(tool_cmd, file)
            
            # Count findings
            if tool_name == "Semgrep":
                count = count_findings(output, "semgrep")
            elif tool_name == "Cppcheck":
                count = count_findings(output, "cppcheck")
            else:  # Flawfinder
                count = count_findings(output, "flawfinder")
            
            results[file][tool_name] = count
            print(f"   Found {count} issue(s)")
            
            # Show first few lines of output
            lines = [l for l in output.split('\n') if l.strip()]
            if lines:
                print(f"   Sample findings:")
                for line in lines[:3]:
                    if len(line) > 100:
                        line = line[:97] + "..."
                    print(f"   {line}")
                if len(lines) > 3:
                    print(f"   ... and {len(lines) - 3} more")
    
    # Summary table
    print("\n" + "="*70)
    print("📊 DETECTION SUMMARY")
    print("="*70)
    print()
    print(f"{'File':<30} {'Semgrep':<15} {'Cppcheck':<15} {'Flawfinder':<15}")
    print("-"*70)
    
    for file in test_files:
        short_name = file.replace('.c', '')[:28]
        sg = results[file].get("Semgrep", 0)
        cc = results[file].get("Cppcheck", 0)
        ff = results[file].get("Flawfinder", 0)
        print(f"{short_name:<30} {sg:<15} {cc:<15} {ff:<15}")
    
    # Calculate totals
    print("-"*70)
    total_sg = sum(r.get("Semgrep", 0) for r in results.values())
    total_cc = sum(r.get("Cppcheck", 0) for r in results.values())
    total_ff = sum(r.get("Flawfinder", 0) for r in results.values())
    print(f"{'TOTAL':<30} {total_sg:<15} {total_cc:<15} {total_ff:<15}")
    
    # Analysis
    print("\n" + "="*70)
    print("🔍 ANALYSIS")
    print("="*70)
    
    print("\n✅ Tool Strengths:")
    print("   • Semgrep: Fast, low false positives, good for common security issues")
    print("   • Cppcheck: Deep analysis, catches undefined behavior and complex bugs")
    print("   • Flawfinder: Security-focused, good risk scoring, finds dangerous functions")
    
    print("\n💡 Recommendations:")
    print("   • Use Semgrep for quick security audits and CI/CD")
    print("   • Use Cppcheck for comprehensive code quality analysis")
    print("   • Use Flawfinder for security-focused reviews")
    print("   • Combine all three for maximum coverage")
    
    print("\n" + "="*70)
    print("✅ Comparison complete!")
    print("="*70)

if __name__ == "__main__":
    main()