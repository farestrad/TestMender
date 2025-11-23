#!/usr/bin/env python3
"""
generate_feedback_prompts.py

Read feedback manifests (CSV) for Semgrep- and Cppcheck-guided harnesses
and produce per-harness JSONL prompts that we can feed to GPT/Gemini
for the TestMender feedback loop.

Input:
  analysis_results/feedback_manifests/semgrep/cweXXX_feedback.csv
  analysis_results/feedback_manifests/cppcheck/cweXXX_feedback.csv

Output:
  analysis_results/feedback_prompts/semgrep/cweXXX_prompts.jsonl
  analysis_results/feedback_prompts/cppcheck/cweXXX_prompts.jsonl
"""

import csv
import json
from pathlib import Path


def reason_hint(reason: str) -> str:
    """Human-readable explanation of the failure mode."""
    if reason == "good_failed":
        return (
            "The GOOD path crashed or exited abnormally. "
            "Fix initialization, preconditions, and buffer sizes so GOOD runs cleanly "
            "with no AddressSanitizer errors or crashes."
        )
    if reason == "bad_passed":
        return (
            "The BAD path did not clearly manifest the vulnerability as expected. "
            "Strengthen the BAD path so it reliably triggers a memory-safety bug that "
            "AddressSanitizer can detect (e.g., overflow/invalid access)."
        )
    # fallback / other categories if added later
    return (
        "The harness behavior did not match the expected pattern where GOOD runs cleanly "
        "and BAD triggers a detectable error."
    )


def build_prompt(
    cwe_id: str,
    harness_code: str,
    harness_name: str,
    source_name: str,
    reason: str,
    good_status: str,
    bad_status: str,
    good_exit: str,
    bad_exit: str,
) -> str:
    """Create the natural-language prompt for the LLM."""
    failure_expl = reason_hint(reason)

    return f"""You previously generated the following C test harness for a Juliet vulnerability:

- CWE ID: CWE{cwe_id}
- Static-analysis source guiding the harness: {source_name}
- Harness file name: {harness_name}

The validator compiled the Juliet code plus this harness with AddressSanitizer enabled,
then ran it in two modes:

- GOOD run: status = {good_status}, exit code = {good_exit}
- BAD run:  status = {bad_status}, exit code = {bad_exit}
- Overall classification: {reason}

Interpretation of this classification:
{failure_expl}

Your task:

1. Rewrite the harness so that:
    - The GOOD path executes to normal completion with **no** AddressSanitizer errors and no crashes.
    - The BAD path reliably triggers the intended bug and causes AddressSanitizer to report an error.
2. Keep using the same Juliet file family and the same GOOD/BAD entry points. Do NOT change which
    Juliet functions are called; only adjust how data is prepared (buffer sizes, contents, lengths,
    control flags, etc.) and any checks inside the harness.
3. Keep the harness as a single self-contained .c file that will be compiled together with the
    existing Juliet sources and testcasesupport.
4. Preserve any necessary #include directives so it still compiles.

Here is the CURRENT harness you wrote:

```c
{harness_code}
Now produce an IMPROVED harness in C that satisfies the goals above.
Return ONLY the complete C source file for the new harness, with no extra explanation.
"""

def process_source(source_name: str, manifests_dir: Path, prompts_dir: Path) -> None:
    """
    Process one source (semgrep or cppcheck):
    - read all cwe*_feedback.csv
    - for each row, load harness code
    - write JSONL prompts file per CWE
    """
    if not manifests_dir.exists():
        print(f"[{source_name}] manifests dir does not exist, skipping: {manifests_dir}")
        return

    prompts_dir.mkdir(parents=True, exist_ok=True)

    # choose base dir where harnesses live
    if source_name == "semgrep":
        harness_base = Path("gpt_sem_generated_test")
    else:
        harness_base = Path("gpt_cppcheck_generated_test")

    for csv_path in sorted(manifests_dir.glob("cwe*_feedback.csv")):
        fname = csv_path.name  # e.g., cwe121_feedback.csv
        cwe_part = fname.split("_", 1)[0]  # "cwe121"
        cwe_id = cwe_part.replace("cwe", "")

        out_jsonl = prompts_dir / f"cwe{cwe_id}_prompts.jsonl"
        rows_written = 0

        with csv_path.open("r", encoding="utf-8") as f_in, \
             out_jsonl.open("w", encoding="utf-8") as f_out:

            reader = csv.DictReader(f_in)
            for row in reader:
                harness_name = row["harness"]
                reason = row["reason"]
                good_status = row["good_status"]
                bad_status = row["bad_status"]
                good_exit = row["good_exit"]
                bad_exit = row["bad_exit"]

                harness_path = harness_base / f"CWE{cwe_id}" / harness_name
                try:
                    harness_code = harness_path.read_text(encoding="utf-8")
                except FileNotFoundError:
                    print(f"[{source_name}] WARNING: harness file not found: {harness_path}")
                    harness_code = ""

                prompt_text = build_prompt(
                    cwe_id=cwe_id,
                    harness_code=harness_code,
                    harness_name=harness_name,
                    source_name=source_name,
                    reason=reason,
                    good_status=good_status,
                    bad_status=bad_status,
                    good_exit=good_exit,
                    bad_exit=bad_exit,
                )

                obj = {
                    "cwe": f"CWE{cwe_id}",
                    "source": source_name,
                    "harness": harness_name,
                    "harness_path": str(harness_path),
                    "reason": reason,
                    "good_status": good_status,
                    "bad_status": bad_status,
                    "good_exit": good_exit,
                    "bad_exit": bad_exit,
                    "prompt": prompt_text,
                }
                f_out.write(json.dumps(obj))
                f_out.write("\n")
                rows_written += 1

        print(f"[{source_name}] Wrote prompts: {out_jsonl} (harnesses: {rows_written})")

def main() -> None:
    # Assuming __file__ is available to determine the script's location
    project_root = Path(__file__).resolve().parent.parent
    manifests_root = project_root / "analysis_results" / "feedback_manifests"
    prompts_root = project_root / "analysis_results" / "feedback_prompts"

    semgrep_manifests = manifests_root / "semgrep"
    cppcheck_manifests = manifests_root / "cppcheck"

    semgrep_prompts = prompts_root / "semgrep"
    cppcheck_prompts = prompts_root / "cppcheck"

    print(f"Project root: {project_root}")
    print(f"Semgrep manifests:   {semgrep_manifests}")
    print(f"Cppcheck manifests:  {cppcheck_manifests}")
    print(f"Prompts output root: {prompts_root}")
    print()

    process_source("semgrep", semgrep_manifests, semgrep_prompts)
    process_source("cppcheck", cppcheck_manifests, cppcheck_prompts)

if __name__ == "__main__":
    main()