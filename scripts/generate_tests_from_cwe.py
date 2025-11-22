#!/usr/bin/env python3
"""
generate_tests_from_cwe.py

- Discovers C files in Juliet under selected CWE directories
- Filters by {bad|good|all}
- Sends each file to Gemini 2.5 Flash asking for tests
- Saves structured JSON + a .c test file for each source

Usage (example):
  python generate_tests_from_cwe.py \
    --cwes 120 121 122 190 22 362 327 256 338 \
    --max-files 25 \
    --match bad \
    --root data/juliet/testcases \
    --out analysis_results/gemini_tests
"""

import argparse
import json
import os
import random
import sys
import time
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable, List, Optional, Tuple

# ---- Gemini SDK ----
# pip install -U google-genai
from google import genai
from google.genai import types


# -------------------------
# Config dataclasses
# -------------------------
@dataclass
class RunConfig:
    root: Path
    cwes: List[int]
    match: str               # 'bad' | 'good' | 'all'
    max_files: int
    shuffle: bool
    seed: Optional[int]
    outdir: Path
    model: str
    temperature: float


# -------------------------
# File discovery
# -------------------------
def find_cwe_dirs(root: Path, cwes: Iterable[int]) -> List[Path]:
    found = []
    for n in cwes:
        # Juliet dirs like CWE121_Stack_Based_Buffer_Overflow etc.
        matches = list(root.glob(f"CWE{n}_*"))
        if not matches:
            print(f"⚠️  CWE{n}: not found under {root} (skipping)")
            continue
        # prefer first match; if multiples exist, include all
        found.extend(matches)
    return found


def classify_file(path: Path) -> str:
    """
    Quick filename-based classification.
    Returns 'bad', 'good', or 'unknown'.
    """
    name = path.name.lower()
    if "bad" in name:
        return "bad"
    if "good" in name:
        return "good"
    return "unknown"


def list_c_files(cwe_dirs: List[Path]) -> List[Path]:
    files: List[Path] = []
    for d in cwe_dirs:
        files.extend(d.rglob("*.c"))
    return files


def filter_files(files: List[Path], match: str) -> List[Path]:
    if match == "all":
        return files
    out = []
    for f in files:
        tag = classify_file(f)
        if match == "bad" and tag == "bad":
            out.append(f)
        elif match == "good" and tag == "good":
            out.append(f)
    return out


def select_files(files: List[Path], max_files: int, shuffle: bool, seed: Optional[int]) -> List[Path]:
    files = sorted(files)
    if shuffle:
        rnd = random.Random(seed)
        rnd.shuffle(files)
    if max_files > 0:
        files = files[:max_files]
    return files


# -------------------------
# Gemini interaction
# -------------------------
PROMPT_BRIEF = (
    "Write unit tests for the C code below.\n"
    "Return a SINGLE, self-contained C test file that compiles with gcc on Linux/macOS "
    "(use a tiny custom assert harness if needed). Keep it minimal but meaningful.\n\n"
    "Return strict JSON with fields:\n"
    "- test_code (string)  : full compilable C test file\n"
    "- compile_cmd (string): e.g. gcc -O2 -Wall -Werror -o test_<name> <testfile>.c\n"
    "- run_cmd (string)    : e.g. ./test_<name>\n"
)

def ask_gemini_for_tests(client: genai.Client, model: str, code: str) -> str:
    """
    Returns the response text (JSON string) from Gemini.
    We request JSON mode so .text should already be valid JSON.
    """
    cfg = types.GenerateContentConfig(
        temperature=0.2,
        response_mime_type="application/json",
    )
    prompt = (
        PROMPT_BRIEF
        + "---- BEGIN SOURCE C ----\n"
        + code
        + "\n---- END SOURCE C ----\n"
    )
    resp = client.models.generate_content(
        model=model,
        contents=prompt,
        config=cfg,
    )
    return resp.text  # JSON string (as instructed)


# -------------------------
# Orchestration
# -------------------------
def save_outputs(outdir: Path, source_path: Path, response_json_text: str) -> Tuple[Path, Optional[Path]]:
    """
    Save the raw JSON response + the extracted test C file (if present).
    Returns (json_path, test_c_path or None)
    """
    safe = "__".join(source_path.parts)
    run_dir = outdir / safe
    run_dir.mkdir(parents=True, exist_ok=True)

    # Write raw LLM JSON
    json_path = run_dir / "tests.json"
    json_path.write_text(response_json_text, encoding="utf-8")

    # Try to parse and extract test_code to a .c file
    test_c_path = None
    try:
        payload = json.loads(response_json_text)
        test_code = payload.get("test_code")
        if isinstance(test_code, str) and len(test_code.strip()) > 0:
            test_c_path = run_dir / "test_generated.c"
            test_c_path.write_text(test_code, encoding="utf-8")
    except Exception as e:
        # Save parsing error for debugging
        (run_dir / "parse_error.txt").write_text(str(e), encoding="utf-8")

    return json_path, test_c_path


def main():
    ap = argparse.ArgumentParser(description="Send Juliet C files to Gemini 2.5 Flash for test generation.")
    ap.add_argument("--root", type=Path, default=Path("data/juliet/testcases"),
                    help="Root of Juliet testcases (default: data/juliet/testcases)")
    ap.add_argument("--cwes", type=int, nargs="+", required=True,
                    help="CWE numbers, e.g., 121 122 190 22 362 327 256 338")
    ap.add_argument("--match", choices=["bad", "good", "all"], default="all",
                    help="Which files to include by filename: bad|good|all (default: all)")
    ap.add_argument("--max-files", type=int, default=25,
                    help="Max number of files to send (default: 25)")
    ap.add_argument("--shuffle", action="store_true", help="Shuffle before picking max-files")
    ap.add_argument("--seed", type=int, default=None, help="Random seed if --shuffle")
    ap.add_argument("--out", type=Path, default=Path("analysis_results/gemini_tests"),
                    help="Output directory (default: analysis_results/gemini_tests)")
    ap.add_argument("--model", default="gemini-2.5-flash", help="Gemini model id (default: gemini-2.5-flash)")
    ap.add_argument("--temp", type=float, default=0.2, help="Sampling temperature (default: 0.2)")
    args = ap.parse_args()

    cfg = RunConfig(
        root=args.root,
        cwes=args.cwes,
        match=args.match,
        max_files=args.max_files,
        shuffle=args.shuffle,
        seed=args.seed,
        outdir=args.out,
        model=args.model,
        temperature=args.temp,
    )

    api_key = os.environ.get("GEMINI_API_KEY")
    if not api_key:
        print("❌ Missing GEMINI_API_KEY env var. Set it first: export GEMINI_API_KEY=...", file=sys.stderr)
        sys.exit(1)

    client = genai.Client(api_key=api_key)

    print(f"🔎 Discovering CWE dirs under: {cfg.root}")
    cwe_dirs = find_cwe_dirs(cfg.root, cfg.cwes)
    if not cwe_dirs:
        print("❌ No matching CWE directories found. Exiting.")
        sys.exit(1)

    all_c_files = list_c_files(cwe_dirs)
    print(f"• Found {len(all_c_files)} total .c files in selected CWEs")

    filtered = filter_files(all_c_files, cfg.match)
    print(f"• After match='{cfg.match}': {len(filtered)} files")

    chosen = select_files(filtered, cfg.max_files, cfg.shuffle, cfg.seed)
    print(f"• Will send {len(chosen)} file(s) to model: {cfg.model}")
    cfg.outdir.mkdir(parents=True, exist_ok=True)

    success, failures = 0, 0
    start_ts = time.strftime("%Y-%m-%d %H:%M:%S")
    print(f"⏱️  Start: {start_ts}\n")

    for idx, cfile in enumerate(chosen, 1):
        rel = cfile
        try:
            code = cfile.read_text(encoding="utf-8", errors="ignore")
        except Exception as e:
            print(f"[{idx}/{len(chosen)}] ❌ Read failed: {rel} -> {e}")
            failures += 1
            continue

        print(f"[{idx}/{len(chosen)}] ✉️  Sending: {rel}")
        try:
            resp_text = ask_gemini_for_tests(client, cfg.model, code)
        except Exception as e:
            print(f"   ❌ API error: {e}")
            failures += 1
            continue

        json_path, test_c_path = save_outputs(cfg.outdir, rel, resp_text)
        print(f"   ✅ Saved: {json_path}")
        if test_c_path:
            print(f"   🧪 Test C: {test_c_path}")
        else:
            print(f"   ⚠️ No 'test_code' extracted (check JSON).")
        success += 1

    end_ts = time.strftime("%Y-%m-%d %H:%M:%S")
    print(f"\n✅ Done. Success: {success}, Failures: {failures}")
    print(f"⏹️  End: {end_ts}")
    print(f"📁 Outputs in: {cfg.outdir.resolve()}")


if __name__ == "__main__":
    main()
