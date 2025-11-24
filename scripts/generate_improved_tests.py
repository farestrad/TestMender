#!/usr/bin/env python3
import os
import json
from pathlib import Path
from openai import OpenAI

# -----------------------------
# CONFIG
# -----------------------------
MODEL = "gpt-4.1"
PROJECT_ROOT = Path(__file__).resolve().parent.parent

PROMPTS_ROOT = PROJECT_ROOT / "analysis_results" / "feedback_prompts"
OUTPUT_ROOT = PROJECT_ROOT / "improved_tests"

# Create a global client (relies on OPENAI_API_KEY in the env)
client = OpenAI()


# -----------------------------
# CREATE OUTPUT DIRS
# -----------------------------
def ensure_dir(path: Path):
    path.mkdir(parents=True, exist_ok=True)


# -----------------------------
# SEND ONE PROMPT TO GPT
# -----------------------------
def call_gpt(prompt_text: str) -> str:
    """
    Uses the new Responses API.
    We send a single user message and get back plain text.
    """
    resp = client.responses.create(
        model=MODEL,
        input=[
            {
                "role": "system",
                "content": "You are an expert C test harness generator. Output ONLY the corrected C code."
            },
            {
                "role": "user",
                "content": prompt_text
            }
        ]
    )

    # New SDK: text output is in resp.output[0].content[0].text
    return resp.output[0].content[0].text.strip()


# -----------------------------
# PROCESS ONE JSONL FILE
# -----------------------------
def process_jsonl(jsonl_path: Path, tool_name: str):
    cwe = jsonl_path.stem.replace("_prompts", "")
    print(f"\n[PROCESS] {tool_name} → {cwe}")

    out_dir = OUTPUT_ROOT / tool_name / cwe
    ensure_dir(out_dir)

    with open(jsonl_path, "r") as f:
        for line in f:
            if not line.strip():
                continue

            item = json.loads(line)

            harness = item["harness"]
            prompt_text = item["prompt"]
            harness_safe = harness.replace(".c", "")

            print(f"  → generating fix for {harness}")

            try:
                improved_code = call_gpt(prompt_text)
            except Exception as e:
                print(f"    !! ERROR calling GPT: {e}")
                continue

            # Save improved harness
            out_file = out_dir / f"{harness_safe}_improved.c"
            with open(out_file, "w") as w:
                w.write(improved_code)

            print(f"    ✓ saved: {out_file}")


# -----------------------------
# MAIN LOOP
# -----------------------------
def main():
    print("=== TestMender: Generating Improved Tests ===")

    for tool in ["semgrep", "cppcheck"]:
        tool_dir = PROMPTS_ROOT / tool
        if not tool_dir.exists():
            print(f"[WARN] No prompt dir: {tool_dir}")
            continue

        for jsonl_file in sorted(tool_dir.glob("*.jsonl")):
            process_jsonl(jsonl_file, tool)

    print("\n=== DONE: Improved tests generated ===")


if __name__ == "__main__":
    main()
