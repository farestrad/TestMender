#!/usr/bin/env python3
"""
extract_feedback_manifest.py

Reads validator JSONL outputs for Semgrep- and Cppcheck-guided harnesses,
extracts ONLY the failing harnesses (validated == false), and writes
per-CWE CSV "feedback manifests" that we will use for the TestMender
feedback loop.

Output structure:

analysis_results/
    feedback_manifests/
        semgrep/
            cwe121_feedback.csv
            cwe122_feedback.csv
            ...
        cppcheck/
            cwe121_feedback.csv
            cwe122_feedback.csv
            ...

Run from project root:
    python3 scripts/extract_feedback_manifest.py
"""

import csv
import json
from pathlib import Path


def extract_for_source(in_dir: Path, out_dir: Path, source_name: str) -> None:
    if not in_dir.exists() or not in_dir.is_dir():
        print(f"[skip] {source_name}: directory not found: {in_dir}")
        return

    out_dir.mkdir(parents=True, exist_ok=True)

    # Expect files like: cwe121_results.jsonl
    for jsonl_path in sorted(in_dir.glob("cwe*_results.jsonl")):
        filename = jsonl_path.name  # e.g., cwe121_results.jsonl
        # Extract "121" from "cwe121_results.jsonl"
        try:
            cwe_part = filename.split("_", 1)[0]  # "cwe121"
            cwe_id = cwe_part.replace("cwe", "")
        except Exception:
            print(f"[warn] {source_name}: cannot parse CWE id from {filename}, skipping.")
            continue

        out_csv = out_dir / f"cwe{cwe_id}_feedback.csv"
        num_rows = 0

        with jsonl_path.open("r", encoding="utf-8") as f_in, \
             out_csv.open("w", encoding="utf-8", newline="") as f_out:

            writer = csv.writer(f_out)
            writer.writerow([
                "harness",
                "reason",
                "good_status",
                "bad_status",
                "good_exit",
                "bad_exit",
                "source"
            ])

            for line in f_in:
                line = line.strip()
                if not line:
                    continue
                try:
                    obj = json.loads(line)
                except json.JSONDecodeError as e:
                    print(f"[warn] {source_name}: JSON decode error in {jsonl_path}: {e}")
                    continue

                # Only failing harnesses → for feedback
                if obj.get("validated", True):
                    continue  # skip validated ones

                harness     = obj.get("harness", "")
                reason      = obj.get("reason", "")
                good_status = obj.get("good_status", "")
                bad_status  = obj.get("bad_status", "")
                good_exit   = obj.get("good_exit", "")
                bad_exit    = obj.get("bad_exit", "")

                writer.writerow([
                    harness,
                    reason,
                    good_status,
                    bad_status,
                    good_exit,
                    bad_exit,
                    source_name,
                ])
                num_rows += 1

        print(f"[{source_name}] {jsonl_path} → {out_csv} (failing harnesses: {num_rows})")


def main() -> None:
    project_root = Path(__file__).resolve().parent.parent

    # Input dirs (what your validator scripts wrote)
    semgrep_json_dir   = project_root / "analysis_results" / "validator_json"
    cppcheck_json_dir  = project_root / "analysis_results" / "validator_json_cppcheck"

    # Output dirs (new)
    manifests_base     = project_root / "analysis_results" / "feedback_manifests"
    semgrep_out_dir    = manifests_base / "semgrep"
    cppcheck_out_dir   = manifests_base / "cppcheck"

    print(f"Project root: {project_root}")
    print(f"Semgrep JSONL dir:   {semgrep_json_dir}")
    print(f"Cppcheck JSONL dir:  {cppcheck_json_dir}")
    print(f"Output manifests:    {manifests_base}")
    print()

    extract_for_source(semgrep_json_dir,  semgrep_out_dir,  "semgrep")
    extract_for_source(cppcheck_json_dir, cppcheck_out_dir, "cppcheck")


if __name__ == "__main__":
    main()
