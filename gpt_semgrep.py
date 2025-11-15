#!/usr/bin/env python3
"""
gpt_semgrep.py

Integrates precomputed Semgrep results with GPT-based test generation.

- Uses gpt-4o-mini.
- Input: Semgrep JSON + Juliet C files.
- Output: C test harnesses saved as .c files.

Typical usage (from TestMender repo root):

  python3 gpt_semgrep.py 121 \
    --semgrep-dir "analysis_results/semgrep/CWE121_20251109_005446/full_results" \
    --repo-root "." \
    --output-dir "gpt_sem_generated_test" \
    --prev-tests-dir "generated_test_cases/CWE121"

Author: Emmanuel + ChatGPT
"""

import argparse
import json
from pathlib import Path
from collections import defaultdict
from openai import OpenAI

client = OpenAI()  # reads OPENAI_API_KEY from your environment


# ------------------------------------------------------------
# SEMGREP HELPERS
# ------------------------------------------------------------

def load_semgrep_json(semgrep_dir: Path) -> dict:
    """
    Load the first JSON file inside a Semgrep result directory.

    Example:
      semgrep_dir = analysis_results/semgrep/CWE121_20251109_005446/full_results
      JSON inside: CWE121_20251109_005446.json
    """
    if not semgrep_dir.is_dir():
        raise FileNotFoundError(f"Semgrep directory not found: {semgrep_dir}")

    # Use rglob to be robust to nested JSON files
    json_files = sorted(semgrep_dir.rglob("*.json"))
    if not json_files:
        raise FileNotFoundError(f"No JSON files found in {semgrep_dir}")

    json_path = json_files[0]
    with json_path.open("r", encoding="utf-8") as f:
        data = json.load(f)

    print(f"[+] Loaded Semgrep JSON: {json_path}")
    return data


def group_findings_by_path(semgrep_data: dict):
    """
    Group Semgrep results by file path.

    semgrep_data["results"] is a list of matches, each with fields like:
      - "path"
      - "start" { "line": ..., "col": ... }
      - "extra" { "rule": ..., "message": ..., "severity": ... }

    Returns:
      dict[path_str -> list[issue_dict]]
    """
    grouped = defaultdict(list)
    for issue in semgrep_data.get("results", []):
        path = issue.get("path")
        if path:
            grouped[path].append(issue)
    return grouped


def summarize_findings(path: str, issues, max_items: int = 20) -> str:
    """
    Human-readable summary of Semgrep findings for GPT.
    """
    if not issues:
        return f"No Semgrep findings were reported for this file ({path})."

    lines = [f"Semgrep findings for {path}:"]
    for issue in issues[:max_items]:
        extra = issue.get("extra", {})
        msg = extra.get("message", "")
        rule = extra.get("rule", "")
        severity = extra.get("severity", "")
        start = issue.get("start", {})
        line = start.get("line", "?")
        col = start.get("col", "?")

        lines.append(f"- [{severity}] {rule} at line {line}, col {col}: {msg}")

    if len(issues) > max_items:
        lines.append(f"... (truncated {len(issues) - max_items} additional findings)")

    return "\n".join(lines)


# ------------------------------------------------------------
# NAME MATCHING HELPERS
# ------------------------------------------------------------

def canonical_stem(name: str) -> str:
    """
    Normalize a filename stem so we can match between:
      - original Juliet file stems
      - previously generated test file stems

    Strips common suffixes like _gpt, _tests, _test, _semgrep, _gen, _harness.

    Example:
      'CWE121_..._05_harness' -> 'CWE121_..._05'
      'CWE121_..._05_gpt'     -> 'CWE121_..._05'
    """
    suffixes = ["_gpt", "_tests", "_test", "_semgrep", "_gen", "_harness"]
    base = name
    for suf in suffixes:
        if base.endswith(suf):
            base = base[:-len(suf)]
    return base


def load_previous_stems(prev_tests_dir: Path) -> set:
    """
    Look at all files in generated_test_cases/CWE<id> and build a set of
    canonical stems to restrict which Juliet files we regenerate.

    Example:
      generated_test_cases/CWE121/CWE121_..._05_harness.c
      -> stem 'CWE121_..._05_harness' -> canonical 'CWE121_..._05'
    """
    if not prev_tests_dir.is_dir():
        raise FileNotFoundError(f"Previous tests directory not found: {prev_tests_dir}")

    stems = set()
    for f in prev_tests_dir.iterdir():
        if f.is_file():
            stem = f.stem  # filename without extension
            stems.add(canonical_stem(stem))

    print(f"[+] Loaded {len(stems)} canonical stems from {prev_tests_dir}")
    return stems


# ------------------------------------------------------------
# PROMPT + GPT
# ------------------------------------------------------------

def build_prompt(source: str, semgrep_summary: str, cwe_id: str, path: str) -> str:
    """
    Creates the full prompt fed to GPT.

    We explicitly ask for ONLY C code output, suitable to save as a .c file.
    """
    return f"""
You are a C programmer and security-focused test generator.

We are analyzing a Juliet test suite file for CWE-{cwe_id}:

File: {path}

You are given:
 - The vulnerable or reference C source code
 - Semgrep static analysis findings for this file

Your task:

1. Generate a **single compilable C source file** that contains:
   - Any necessary #include directives
   - A `main()` function
   - One or more test functions or test cases that:
       * exercise the code paths relevant to CWE-{cwe_id}
       * especially target the potential issues pointed out by Semgrep
   - Print statements or simple checks (e.g., if conditions, assertions) that
     make it clear when a test exposes a bug or unexpected behavior.

2. IMPORTANT OUTPUT FORMAT:
   - **Output ONLY valid C code.**
   - Do NOT include Markdown.
   - Do NOT include explanations or commentary outside of normal C comments
     (i.e., only `/* ... */` and `// ...` inside the C file).
   - The result should be ready to save as a `.c` file and compile.

You may:
 - Call the original Juliet functions if they are visible.
 - Or wrap the vulnerable pattern in a small harness in this file.

======================= ORIGINAL SOURCE CODE =======================
{source}
===================== END ORIGINAL SOURCE CODE =====================

===================== SEMGREP FINDINGS (GUIDANCE) ==================
{semgrep_summary}
================== END SEMGREP FINDINGS (GUIDANCE) =================
"""


def call_gpt(prompt: str) -> str:
    """
    Call GPT-4o-mini and return the generated text (expected to be C code).
    """
    resp = client.responses.create(
        model="gpt-4o-mini",
        input=prompt,
    )
    return resp.output[0].content[0].text


# ------------------------------------------------------------
# MAIN PIPELINE
# ------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(
        description="Generate C test files using Semgrep analysis + GPT (CWE-specific)."
    )
    parser.add_argument("cwe_id", type=str, help="Example: 121")
    parser.add_argument(
        "--semgrep-dir",
        required=True,
        help="Directory where Semgrep JSON for this CWE is stored "
             "(e.g. analysis_results/semgrep/CWE121_.../full_results).",
    )
    parser.add_argument(
        "--repo-root",
        default=".",
        help="Root of TestMender repo (where 'data/juliet/...' lives).",
    )
    parser.add_argument(
        "--output-dir",
        default="gpt_sem_generated_test",
        help="Root directory to store GPT-generated C test files.",
    )
    parser.add_argument(
        "--max-files",
        type=int,
        default=0,
        help="Optional: maximum number of files to process (0 = all).",
    )
    parser.add_argument(
        "--prev-tests-dir",
        type=str,
        default="",
        help="Directory of previously generated tests for this CWE "
             "(e.g. generated_test_cases/CWE121). If set, only Juliet files "
             "whose stem matches a file in this directory will be processed.",
    )

    args = parser.parse_args()

    repo_root = Path(args.repo_root)
    semgrep_dir = Path(args.semgrep_dir)
    output_root = Path(args.output_dir)
    cwe_id = args.cwe_id
    prev_tests_dir = Path(args.prev_tests_dir) if args.prev_tests_dir else None

    # Step 0: Optional filter based on previous tests
    prev_stems = None
    if prev_tests_dir:
        prev_stems = load_previous_stems(prev_tests_dir)

    # Step 1: Load Semgrep data
    semgrep_data = load_semgrep_json(semgrep_dir)
    findings_by_path = group_findings_by_path(semgrep_data)

    # Step 2: Determine which files to process from Semgrep JSON
    scanned_paths = semgrep_data.get("paths", {}).get("scanned", [])
    if not scanned_paths:
        scanned_paths = list(findings_by_path.keys())

    if not scanned_paths:
        print(f"[!] No scanned paths found in Semgrep JSON for CWE{cwe_id}")
        return

    # Step 2b: If previous tests exist, filter to only matching stems
    if prev_stems is not None:
        filtered_paths = []
        for p in scanned_paths:
            juliet_stem = canonical_stem(Path(p).stem)
            if juliet_stem in prev_stems:
                filtered_paths.append(p)
        print(f"[+] Filtered scanned paths using previous tests: "
              f"{len(filtered_paths)}/{len(scanned_paths)} matched")
        scanned_paths = filtered_paths

        if not scanned_paths:
            print("[!] After filtering with prev-tests-dir, no files remain to process.")
            return

    # Apply optional max-files limit
    if args.max_files and args.max_files > 0:
        scanned_paths = scanned_paths[:args.max_files]

    total_files = len(scanned_paths)
    print(f"[+] Files to process for CWE{cwe_id}: {total_files}")

    # Output directory for this CWE
    out_cwe_dir = output_root / f"CWE{cwe_id}"
    out_cwe_dir.mkdir(parents=True, exist_ok=True)

    # Step 3: Process each file with progress indicator
    for idx, rel_path in enumerate(scanned_paths, start=1):
        print(f"\n[>] ({idx}/{total_files}) Processing {rel_path}")

        src_path = repo_root / rel_path

        if not src_path.is_file():
            print(f"[!] Skipping {rel_path}: source file not found at {src_path}")
            continue

        try:
            source_code = src_path.read_text(encoding="utf-8", errors="ignore")
        except Exception as e:
            print(f"[!] Could not read {src_path}: {e}")
            continue

        issues = findings_by_path.get(rel_path, [])
        semgrep_summary = summarize_findings(rel_path, issues)

        prompt = build_prompt(source_code, semgrep_summary, cwe_id, rel_path)

        print(f"[+] Calling GPT for {rel_path}...")
        try:
            gpt_output = call_gpt(prompt)
        except Exception as e:
            print(f"[!] GPT call failed for {rel_path}: {e}")
            continue

        # Build a clean output filename:
        # Take the original file name, strip .c, append _gpt.c
        stem = Path(rel_path).stem           # e.g., "CWE121_..._01"
        safe_stem = stem.replace("/", "_").replace("\\", "_")
        out_path = out_cwe_dir / f"{safe_stem}_gpt.c"

        try:
            out_path.write_text(gpt_output, encoding="utf-8")
        except Exception as e:
            print(f"[!] Failed to write {out_path}: {e}")
            continue

        print(f"[✓] Saved GPT-generated C file: {out_path}")

    print(f"\nDONE — C test files saved under: {out_cwe_dir}\n")


if __name__ == "__main__":
    main()
