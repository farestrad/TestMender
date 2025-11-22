#!/usr/bin/env python3
"""
gpt_cppcheck.py

Integrates precomputed Cppcheck results with GPT-based *harness* generation.

- Uses gpt-4o-mini.
- Input: Cppcheck JSON + Juliet C files.
- Output: C test harnesses saved as .c files ( *_harness.c ).

Typical usage (from TestMender repo root):

  python3 gpt_cppcheck.py 122 \
    --cppcheck-dir "analysis_results/cppcheck/CWE122_20251121_184407/full_results" \
    --repo-root "." \
    --output-dir "gpt_cppcheck_generated_test" \
    --prev-tests-dir "generated_test_cases/CWE122"

Assumes Cppcheck JSON format produced by run_cppcheck.sh:
{
  "tool": "cppcheck",
  "cwe": "122",
  "source_dir": "...",
  "timestamp": "...",
  "findings": [
    {
      "id": "...",
      "severity": "...",
      "msg": "...",
      "verbose": "...",
      "cwe": "...",
      "locations": [
        {
          "file": "data/juliet/testcases/...",
          "line": "123",
          "column": "4",
          "info": "..."
        },
        ...
      ]
    },
    ...
  ]
}

Author: Fares + ChatGPT
"""

import argparse
import json
from pathlib import Path
from collections import defaultdict
from openai import OpenAI

client = OpenAI()  # reads OPENAI_API_KEY from your environment


# ------------------------------------------------------------
# CPPCHECK HELPERS
# ------------------------------------------------------------

def load_selected_list(path: Path) -> set[str]:
    """
    Load a list of Juliet files or stems from JSON or text.

    - JSON:
        * list of strings
        * or { "selected_files": [ ... ] }
        * or { "files": [ ... ] }
    - Text: one path/stem per line, comments allowed with '#'.
    """
    if not path.is_file():
        raise FileNotFoundError(f"selected-files path not found: {path}")

    raw_list = []

    if path.suffix.lower() == ".json":
        data = json.loads(path.read_text(encoding="utf-8"))
        if isinstance(data, list):
            raw_list = data
        elif isinstance(data, dict):
            if "selected_files" in data:
                raw_list = data["selected_files"]
            elif "files" in data:
                raw_list = data["files"]
    else:
        for line in path.read_text(encoding="utf-8").splitlines():
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            raw_list.append(line)

    stems = set()
    for entry in raw_list:
        stem = canonical_stem(Path(str(entry)).stem)
        stems.add(stem)

    print(f"[+] Loaded {len(stems)} selected stems from {path}")
    return stems




def load_cppcheck_json(cpp_path: Path) -> dict:
    """
    Load Cppcheck JSON.

    - If cpp_path is a file: load that file directly.
    - If cpp_path is a directory: load the first *.json inside it.
    """
    if cpp_path.is_file():
        json_path = cpp_path
    elif cpp_path.is_dir():
        json_files = sorted(cpp_path.rglob("*.json"))
        if not json_files:
            raise FileNotFoundError(f"No JSON files found in {cpp_path}")
        json_path = json_files[0]
    else:
        raise FileNotFoundError(f"Cppcheck path not found: {cpp_path}")

    with json_path.open("r", encoding="utf-8") as f:
        data = json.load(f)

    print(f"[+] Loaded Cppcheck JSON: {json_path}")
    return data



def group_cppcheck_by_file(cpp_data: dict):
    """
    Group Cppcheck findings by *file* (location.file).

    cpp_data["findings"] is a list of issues, each with:
      - "id", "severity", "msg", "verbose", "cwe"
      - "locations": [ { "file", "line", "column", "info" }, ... ]

    Returns:
      dict[file_path_str -> list[issue_dict]]

    Each issue_dict in the grouped lists is flattened to include location info:
      {
        "id": ...,
        "severity": ...,
        "msg": ...,
        "cwe": ...,
        "line": int or None,
        "column": int or None,
        "info": str
      }
    """
    grouped = defaultdict(list)
    for finding in cpp_data.get("findings", []):
        base_issue = {
            "id": finding.get("id", ""),
            "severity": finding.get("severity", ""),
            "msg": finding.get("msg", ""),
            "verbose": finding.get("verbose", ""),
            "cwe": finding.get("cwe", ""),
        }
        locations = finding.get("locations", []) or []
        for loc in locations:
            path = loc.get("file")
            if not path:
                continue
            issue_entry = dict(base_issue)
            # line / column may be strings; normalize to int where possible
            line_raw = loc.get("line")
            col_raw = loc.get("column")
            try:
                issue_entry["line"] = int(line_raw) if line_raw is not None else None
            except (TypeError, ValueError):
                issue_entry["line"] = None
            try:
                issue_entry["column"] = int(col_raw) if col_raw is not None else None
            except (TypeError, ValueError):
                issue_entry["column"] = None
            issue_entry["info"] = loc.get("info", "")
            grouped[path].append(issue_entry)
    return grouped


def summarize_cppcheck_findings(path: str, issues, max_items: int = 20) -> str:
    """
    Human-readable summary of Cppcheck findings for GPT.
    """
    if not issues:
        return f"No Cppcheck findings were reported for this file ({path})."

    lines = [f"Cppcheck findings for {path}:"]
    for issue in issues[:max_items]:
        sev = issue.get("severity", "")
        fid = issue.get("id", "")
        msg = issue.get("msg", "")
        cwe = issue.get("cwe", "")
        line = issue.get("line")
        col = issue.get("column")
        extra = issue.get("info", "")

        loc_str = []
        if line is not None:
            loc_str.append(f"line {line}")
        if col is not None:
            loc_str.append(f"col {col}")
        loc_str = ", ".join(loc_str) if loc_str else "unknown location"

        cwe_str = f" (CWE-{cwe})" if cwe else ""
        lines.append(
            f"- [{sev}] {fid}{cwe_str} at {loc_str}: {msg} {('(info: ' + extra + ')') if extra else ''}".rstrip()
        )

    if len(issues) > max_items:
        lines.append(f"... (truncated {len(issues) - max_items} additional findings)")

    return "\n".join(lines)


# ------------------------------------------------------------
# NAME MATCHING HELPERS (same as Semgrep script)
# ------------------------------------------------------------

def canonical_stem(name: str) -> str:
    """
    Normalize a filename stem so we can match between:
      - original Juliet file stems
      - previously generated test file stems

    Strips common suffixes like _gpt, _tests, _test, _semgrep, _gen, _harness.
    """
    suffixes = ["_gpt", "_tests", "_test", "_semgrep", "_gen", "_harness"]
    base = name
    for suf in suffixes:
        if base.endswith(suf):
            base = base[:-len(suf)]
    return base


def load_previous_stems(prev_tests_dir: Path) -> set:
    """
    Look at all files in generated_test_cases/CWE<id> (or similar) and build a set
    of canonical stems to restrict which Juliet files we regenerate harnesses for.
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
# PROMPT + GPT (adapted to Cppcheck)
# ------------------------------------------------------------

def build_prompt(source: str, cppcheck_summary: str, cwe_id: str, path: str) -> str:
    """
    Creates the full prompt fed to GPT.

    Explicitly asks for a *harness* file, not a full re-implementation.
    """
    return f"""
You are a C programmer and security-focused test harness author.

We are analyzing a Juliet test suite file for CWE-{cwe_id}:

  File: {path}

You are given:
 - The vulnerable or reference C source code (Juliet-style file)
 - Cppcheck static analysis findings for this file

Your task is to generate a **C TEST HARNESS** for this file, *not* a copy of
the entire Juliet implementation.

Requirements for the harness:

1. The harness must be a small driver that calls into the original Juliet
   GOOD/BAD entrypoints.

   - Declare prototypes for the Juliet entry functions that this harness will
     call. For typical Juliet files these look like, for example:

         void CWE{cwe_id}_..._bad(void);
         void CWE{cwe_id}_..._good(void);

     or similar GOOD/BAD naming. Infer the correct names from the source code
     you see below.

   - Implement:

         void run_bad(void);
         void run_good(void);

     where:
       * run_bad() calls the BAD Juliet entry function and sets up any inputs
         needed to exercise the CWE-{cwe_id} behavior (especially those hinted
         at by Cppcheck).
       * run_good() calls the GOOD Juliet entry function and similarly drives
         the safe path.

   - Implement:

         int main(void) {{
         #ifdef TEST_MODE_BAD
             run_bad();
         #else
             run_good();
         #endif
             return 0;
         }}

     This allows an external harness script to compile the same C file twice:
       - once with -DTEST_MODE_BAD (BAD mode)
       - once without that macro (GOOD mode).

2. Observability:
   - It is fine to add printf / wprintf calls or simple checks/conditions in
     run_bad() / run_good() to make it obvious that the code paths have run,
     and to help ASan or other tools surface memory safety issues.
   - Do NOT attempt to fully rewrite or duplicate the Juliet source; treat it
     as the "code under test" and just drive it.

3. IMPORTANT OUTPUT FORMAT:
   - Output ONLY valid, standalone C code for the harness.
   - Do NOT include Markdown.
   - Do NOT include explanations or commentary outside of normal C comments
     (i.e., only /* ... */ and // ... inside the C file).

Use the original source and Cppcheck findings below ONLY as guidance for:
  - which functions are the GOOD/BAD entrypoints,
  - which parameters / conditions trigger the vulnerability,
  - what edge cases to exercise.

======================= ORIGINAL SOURCE CODE =======================
{source}
===================== END ORIGINAL SOURCE CODE =====================

================== CPPCHECK FINDINGS (GUIDANCE) ===================
{cppcheck_summary}
================ END CPPCHECK FINDINGS (GUIDANCE) ================
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
        description="Generate C *harness* files using Cppcheck analysis + GPT (CWE-specific)."
    )
    parser.add_argument("cwe_id", type=str, help="Example: 121")
    parser.add_argument(
        "--cppcheck-dir",
        required=True,
        help="Directory where Cppcheck JSON for this CWE is stored "
             "(e.g. analysis_results/cppcheck/CWE121_.../full_results).",
    )
    parser.add_argument(
        "--repo-root",
        default=".",
        help="Root of TestMender repo (where 'data/juliet/...' lives).",
    )
    parser.add_argument(
        "--output-dir",
        default="gpt_cppcheck_generated_test",
        help="Root directory to store GPT-generated C harness files.",
    )
    parser.add_argument(
        "--max-files",
        type=int,
        default=0,
        help="Optional: maximum number of files to process (0 = all).",
    )

    parser.add_argument(
        "--selected-files",
        type=str,
        default="",
        help="Optional JSON/TXT list of Juliet files used in the Semgrep+GPT run "
             "(e.g. generated_test_cases/CWE122/selected_files.json). "
             "If set, restricts Cppcheck-guided generation to exactly those files.",
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
    cpp_dir = Path(args.cppcheck_dir)
    output_root = Path(args.output_dir)
    cwe_id = args.cwe_id
    prev_tests_dir = Path(args.prev_tests_dir) if args.prev_tests_dir else None

    selected_stems = None
    if args.selected_files:
        selected_stems = load_selected_list(Path(args.selected_files))


    # Step 0: Optional filter based on previous tests
    prev_stems = None
    if prev_tests_dir:
        prev_stems = load_previous_stems(prev_tests_dir)

    # Step 1: Load Cppcheck data
    cpp_data = load_cppcheck_json(cpp_dir)
    findings_by_file = group_cppcheck_by_file(cpp_data)

    scanned_paths = sorted(findings_by_file.keys())
    if not scanned_paths:
        print(f"[!] No findings in Cppcheck JSON for CWE{cwe_id}")
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

    # Step 2c: Filter using selected-files.json
    if selected_stems is not None:
        filtered2 = []
        for p in scanned_paths:
            if canonical_stem(Path(p).stem) in selected_stems:
                filtered2.append(p)
        print(f"[+] Filtered scanned paths using selected-files: "
            f"{len(filtered2)}/{len(scanned_paths)} matched")
        scanned_paths = filtered2

        if not scanned_paths:
            print("[!] After filtering with selected-files, no files remain to process.")
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

        issues = findings_by_file.get(rel_path, [])
        cppcheck_summary = summarize_cppcheck_findings(rel_path, issues)

        prompt = build_prompt(source_code, cppcheck_summary, cwe_id, rel_path)

        print(f"[+] Calling GPT for {rel_path}...")
        try:
            gpt_output = call_gpt(prompt)
        except Exception as e:
            print(f"[!] GPT call failed for {rel_path}: {e}")
            continue

        # Build a clean output filename:
        # Take the original file name, strip .c, append _harness.c
        stem = Path(rel_path).stem           # e.g., "CWE122_..._01"
        safe_stem = stem.replace("/", "_").replace("\\", "_")
        out_path = out_cwe_dir / f"{safe_stem}_harness.c"

        try:
            out_path.write_text(gpt_output, encoding="utf-8")
        except Exception as e:
            print(f"[!] Failed to write {out_path}: {e}")
            continue

        print(f"[✓] Saved GPT-generated harness C file: {out_path}")

    print(f"\nDONE — C harness files saved under: {out_cwe_dir}\n")


if __name__ == "__main__":
    main()
