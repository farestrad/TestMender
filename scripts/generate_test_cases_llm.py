#!/usr/bin/env python3
"""
generate_test_cases_llm.py

Generate *LLM-authored* test artifacts for Juliet C files.

Workflow per selected CWE:
  1) Stable-randomly pick up to 20 files (persisted after first run).
  2) For each file, try in order:
     a) Ask model for a tiny **C harness** calling GOOD/BAD that enforces:
        GOOD passes, BAD fails (child process + alarm()).
     b) If (a) fails, ask for **strict JSON** test specs (5 items).
     c) If (b) fails, ask for **simple text** test ideas.

Providers:
  - Google Gemini (default): --provider google --model gemini-2.5-flash
  - OpenAI GPT-4o mini:      --provider openai --model gpt-4o-mini
"""

import os
import sys
import json
import random
import time
import argparse
import re
from pathlib import Path
from datetime import datetime
from dotenv import load_dotenv

# Optional imports (loaded lazily inside call_model):
# - Google:   from google import genai
# - OpenAI:   from openai import OpenAI

# ------------------------------------------------------------------------------
# Env & basic config
# ------------------------------------------------------------------------------

load_dotenv()

ALLOWED_CWES = [121, 122, 835, 364, 190, 476, 665, 546]
FILES_PER_CWE = 20
JULIET_BASE = Path("data/juliet/testcases")
OUTPUT_DIR = Path("generated_test_cases")

DEFAULT_PROVIDER = "google"       # "google" or "openai"
DEFAULT_GOOGLE_MODEL = "gemini-2.5-flash"
DEFAULT_OPENAI_MODEL = "gpt-4o-mini"

# ------------------------------------------------------------------------------
# Prompt blocks
# ------------------------------------------------------------------------------

RULES_JSON = """You are generating security test cases for a single Juliet C file.

OUTPUT RULES (must follow):
- Return ONE RFC8259-compliant JSON object ONLY (no markdown, no prose).
- JSON schema: {"test_cases": [{"test_id": int, "description": str, "input_data": str, "bad_expected": str, "good_expected": str, "validation_method": str}, ... x5]}
- test_id must be integers 1..5 with no gaps.
- Be concise. No trailing commas. No comments.
"""

RULES_C = """You are generating a standalone C test harness for one Juliet file.

Rules (MUST follow):
- Output ONLY valid C code. No markdown, no comments, no explanations.
- Must compile on macOS/Linux with clang or gcc.
- Include: #include <stdio.h>, <stdlib.h>, <signal.h>, <unistd.h>, <sys/wait.h>, <errno.h>.
- Declare the existing Juliet entry points exactly as:
    extern void {good_sym}(void);
    extern void {bad_sym}(void);
- In main():
    * Fork and run GOOD in the child with alarm(3). Expect: normal exit status 0 (no signal).
    * Fork and run BAD  in the child with alarm(3). Expect: abnormal termination (non-zero or signal) OR timeout.
    * Print exactly one line for each: "GOOD: PASS" or "GOOD: FAIL" and "BAD: PASS" or "BAD: FAIL".
    * Exit 0 only if GOOD: PASS AND BAD: PASS. Otherwise exit 1.
- Do NOT include the implementation of {good_sym}/{bad_sym}.
"""

# ------------------------------------------------------------------------------
# File discovery & stable selection
# ------------------------------------------------------------------------------

def find_cwe_files(cwe_number: int):
    cwe_pattern = f"CWE{cwe_number}"
    matching_dirs = list(JULIET_BASE.glob(f"{cwe_pattern}*"))
    if not matching_dirs:
        print(f"⚠️  No directories found for CWE-{cwe_number}")
        return []
    c_files = []
    for cwe_dir in matching_dirs:
        for c_file in cwe_dir.rglob("*.c"):
            if "testcasesupport" in str(c_file):  # exclude helpers
                continue
            if "main" in c_file.name.lower():      # exclude mains
                continue
            c_files.append(c_file)
    return c_files


def load_or_create_selection(cwe_number: int, all_files: list[Path]) -> list[Path]:
    cwe_output_dir = OUTPUT_DIR / f"CWE{cwe_number}"
    cwe_output_dir.mkdir(parents=True, exist_ok=True)
    selection_file = cwe_output_dir / "selected_files.json"

    if selection_file.exists():
        try:
            saved = json.loads(selection_file.read_text())
            saved_paths = [Path(p) for p in saved.get("selected_paths", [])]
            existing = [p for p in saved_paths if p.exists()]
            if existing:
                print(f"📎 Reusing previously selected {len(existing)} files for CWE-{cwe_number}")
                return existing
            else:
                print("ℹ️  Previous selection file found, but some paths missing; re-sampling.")
        except Exception as e:
            print(f"⚠️  Could not read previous selection; re-sampling: {e}")

    num_to_select = min(FILES_PER_CWE, len(all_files))
    selected_files = random.sample(all_files, num_to_select)
    selection_file.write_text(json.dumps({
        "timestamp": datetime.now().isoformat(),
        "selected_paths": [str(p) for p in selected_files]
    }, indent=2))
    print(f"🎲 Randomly selected {num_to_select} files and saved selection for stability.")
    return selected_files

# ------------------------------------------------------------------------------
# Parsing helpers (functions & bodies)
# ------------------------------------------------------------------------------

def extract_file_info(file_path: Path):
    try:
        content = file_path.read_text(errors='ignore')
        good_funcs = re.findall(r'\b(\w*good\w*)\s*\(', content, re.IGNORECASE)
        bad_funcs  = re.findall(r'\b(\w*bad\w*)\s*\(',  content, re.IGNORECASE)
        good_funcs = list(dict.fromkeys(good_funcs))[:5]
        bad_funcs  = list(dict.fromkeys(bad_funcs))[:5]
        return {
            "path": str(file_path),
            "name": file_path.name,
            "content": content,
            "good_functions": good_funcs,
            "bad_functions": bad_funcs,
            "file_size": len(content),
        }
    except Exception as e:
        print(f"   ⚠️  Error reading {file_path}: {e}")
        return None


def _extract_function_body(src: str, fname: str, max_chars: int = 1200) -> str:
    name_pat = re.compile(rf'\b{re.escape(fname)}\s*\(', re.MULTILINE)
    m = name_pat.search(src)
    if not m:
        return ""
    i = m.end() - 1  # at '('
    depth, end_params = 1, None
    while i < len(src) - 1:
        i += 1
        ch = src[i]
        if ch == '(':
            depth += 1
        elif ch == ')':
            depth -= 1
            if depth == 0:
                end_params = i
                break
    if end_params is None:
        return ""
    j = end_params + 1
    while j < len(src) and src[j] in " \t\r\n":
        j += 1
    if j < len(src) and src[j] == ';':  # prototype
        return ""
    if j >= len(src) or src[j] != '{':
        return ""
    brace = 0
    k = j - 1
    while k < len(src) - 1:
        k += 1
        if src[k] == '{':
            brace += 1
        elif src[k] == '}':
            brace -= 1
            if brace == 0:
                body = src[m.start():k+1]
                body = re.sub(r'[ \t]+', ' ', body)
                body = re.sub(r'\n{2,}', '\n', body).strip()
                return body[:max_chars]
    return ""


def _find_function_body_across_chain(file_path: Path, src: str, fname: str, max_chars: int = 1200) -> str:
    body = _extract_function_body(src, fname, max_chars)
    if body:
        return body
    stem = file_path.stem
    base = re.sub(r'(_\d{2}[a-e])$', '', stem, flags=re.IGNORECASE)
    for cand in sorted(file_path.parent.glob(f"{base}_*.c")):
        try:
            csrc = cand.read_text(errors="ignore")
        except Exception:
            continue
        body = _extract_function_body(csrc, fname, max_chars)
        if body:
            return body
    return ""

# ------------------------------------------------------------------------------
# Prompt builders
# ------------------------------------------------------------------------------

def _cwe_hint(cwe_number: int) -> str:
    return {
        121: "Trigger writes larger than the destination stack buffer; expect ASan stack-buffer-overflow or non-zero exit.",
        122: "Trigger heap overrun/mis-sized copy; expect ASan heap-buffer-overflow/UAF.",
        835: "Inputs that prevent loop exit; timeout indicates failure.",
        364: "Induce races between signal handler and main.",
        23:  "Use ../ segments; good should block, bad should allow traversal.",
        36:  "Use absolute paths (/etc/passwd, C:\\Windows\\...); good should sanitize.",
        190: "Near INT_MAX/INT_MIN causing wraparound; good prevents.",
        476: "NULL deref; bad segfaults/ASan, good handles safely."
    }.get(cwe_number, "")


def generate_test_cases_prompt(file_info, cwe_number, bad_bodies=None, good_bodies=None):
    cwe_desc = {
        121: "Stack-Based Buffer Overflow",
        122: "Heap-Based Buffer Overflow",
        835: "Loop with Unreachable Exit Condition (Infinite Loop)",
        364: "Signal Handler Race Condition",
        23:  "Relative Path Traversal",
        36:  "Absolute Path Traversal",
        190: "Integer Overflow or Wraparound",
        476: "NULL Pointer Dereference"
    }.get(cwe_number, f"CWE-{cwe_number}")

    context = {
        "file_name": file_info["name"],
        "cwe": cwe_number,
        "cwe_description": cwe_desc,
        "hint": _cwe_hint(cwe_number),
        "bad_functions": file_info["bad_functions"],
        "good_functions": file_info["good_functions"],
        "bad_function_bodies": (bad_bodies or [])[:2],
        "good_function_bodies": (good_bodies or [])[:2],
    }

    few_shot = {
        "test_cases": [
            {
                "test_id": 1,
                "description": "FORMAT EXAMPLE — replace with real case",
                "input_data": "example",
                "bad_expected": "crash or sanitizer report",
                "good_expected": "normal termination; exit code 0",
                "validation_method": "check non-zero exit or grep ASan output"
            }
        ]
    }

    return (
        "CONTEXT:\n"
        f"{json.dumps(context, indent=2)}\n\n"
        "TASK:\n"
        f"Generate EXACTLY 5 test cases tailored to CWE-{cwe_number} ({cwe_desc}).\n"
        "Requirements:\n"
        "- test_id integers 1..5\n"
        "- fields: description, input_data, bad_expected, good_expected, validation_method\n"
        "- Be concise; JSON only.\n\n"
        "FORMAT EXAMPLE (schema only; replace content):\n"
        f"{json.dumps(few_shot, indent=2)}\n\n"
        "OUTPUT: Return ONLY the final JSON object with 5 test cases."
    )


def build_minimal_prompt(file_info, cwe_number, bad_bodies=None, good_bodies=None):
    cwe_desc = {
        121: "Stack-Based Buffer Overflow",
        122: "Heap-Based Buffer Overflow",
        835: "Loop with Unreachable Exit Condition (Infinite Loop)",
        364: "Signal Handler Race Condition",
        23:  "Relative Path Traversal",
        36:  "Absolute Path Traversal",
        190: "Integer Overflow or Wraparound",
        476: "NULL Pointer Dereference"
    }.get(cwe_number, f"CWE-{cwe_number}")

    parts = []
    parts.append(f"You are a helpful assistant. Give 5 short test ideas for CWE-{cwe_number} ({cwe_desc}).")
    parts.append(f"File: {file_info['name']}")
    if bad_bodies:
        parts.append("Bad sink(s):\n" + "\n\n".join(bad_bodies[:1]))
    if good_bodies:
        parts.append("Good sink(s):\n" + "\n\n".join(good_bodies[:1]))
    parts.append(
        "For each test, include:\n"
        "- Input (one line)\n"
        "- Expected on BAD (one line)\n"
        "- Expected on GOOD (one line)\n"
        "Keep each test to 3 short bullet lines. No code fences, no JSON."
    )
    return "\n\n".join(parts)


def build_c_harness_prompt(file_info):
    good_candidates = [n for n in file_info["good_functions"] if re.search(r'\bgood\b', n, re.IGNORECASE)]
    bad_candidates  = [n for n in file_info["bad_functions"]  if re.search(r'\bbad\b',  n, re.IGNORECASE)]
    good_sym = good_candidates[0] if good_candidates else (file_info["good_functions"][0] if file_info["good_functions"] else None)
    bad_sym  = bad_candidates[0]  if bad_candidates  else (file_info["bad_functions"][0]  if file_info["bad_functions"]  else None)
    if not good_sym or not bad_sym:
        return None, None
    return RULES_C.format(good_sym=good_sym, bad_sym=bad_sym), (good_sym, bad_sym)

# ------------------------------------------------------------------------------
# Model callers (Google + OpenAI)
# ------------------------------------------------------------------------------

def _strip_fences(s: str) -> str:
    s = s.strip()
    if s.startswith("```"):
        s = re.sub(r"^```[a-zA-Z]*\n?", "", s)
        s = re.sub(r"\n?```$", "", s)
    return s.strip()


def _extract_first_json(text: str):
    t = (text or "").strip()
    try:
        return json.loads(t)
    except Exception:
        pass
    stack, start = 0, -1
    for i, ch in enumerate(t):
        if ch == '{':
            if stack == 0:
                start = i
            stack += 1
        elif ch == '}':
            stack -= 1
            if stack == 0 and start != -1:
                cand = t[start:i+1]
                try:
                    return json.loads(cand)
                except Exception:
                    start = -1
    return None


def call_model(provider: str, model_name: str, prompt: str, *, mode: str = "json", simple: bool = False):
    """
    provider: "google" | "openai"
    mode:
      - "c":    expect C code (plain text)
      - "json": expect JSON (enforce structured output where supported)
    simple:
      - True => no structured enforcement; return raw text
    """
    if provider == "google":
        from google import genai
        client = genai.Client(api_key=os.getenv("GEMINI_API_KEY"))
        cfg = {
            "temperature": 0.3 if (simple or mode == "c") else 0.4,
            "top_p": 0.9,
            "top_k": 40,
            "max_output_tokens": 1100 if (simple or mode == "c") else 1200,
        }
        contents = [prompt] if (mode == "c" or simple) else [RULES_JSON, prompt]
        if mode == "json" and not simple:
            cfg["response_mime_type"] = "application/json"
        resp = client.models.generate_content(model=model_name, contents=contents, config=cfg)
        text = (getattr(resp, "text", None) or "").strip()
        if not text:
            return None
        if mode == "c":
            return _strip_fences(text)
        if simple:
            return text
        parsed = _extract_first_json(text)
        return parsed

    elif provider == "openai":
        from openai import OpenAI
        client = OpenAI(api_key=os.getenv("OPENAI_API_KEY"))
        if not os.getenv("OPENAI_API_KEY"):
            print("❌ OPENAI_API_KEY not set. Export it to use --provider openai.")
            return None

        # Responses API with optional JSON response_format
        # (Official docs: Text generation + Structured outputs)
        kwargs = {
            "model": model_name,
            "input": prompt,
            "temperature": 0.3 if (simple or mode == "c") else 0.4,
        }
        if mode == "json" and not simple:
            kwargs["response_format"] = {"type": "json_object"}

        resp = client.responses.create(**kwargs)
        # Most SDKs expose a convenience property for full text:
        text = getattr(resp, "output_text", None)
        if not text:
            # Fallback: stitch from content parts
            try:
                parts = []
                for item in resp.output:
                    if item.type == "message":
                        for c in item.message.content:
                            if c.type == "output_text":
                                parts.append(c.text)
                    elif item.type == "output_text":
                        parts.append(item.text)
                text = "".join(parts).strip()
            except Exception:
                text = ""
        if not text:
            return None

        if mode == "c":
            return _strip_fences(text)
        if simple:
            return text
        parsed = _extract_first_json(text)
        return parsed

    else:
        print(f"❌ Unknown provider: {provider}")
        return None

# ------------------------------------------------------------------------------
# Main pipeline
# ------------------------------------------------------------------------------

def process_cwe(cwe_number: int, provider: str, model_name: str):
    print(f"\n{'='*70}")
    print(f"Processing CWE-{cwe_number} with {provider}:{model_name}")
    print(f"{'='*70}")

    all_files = find_cwe_files(cwe_number)
    print(f"📁 Found {len(all_files)} C files for CWE-{cwe_number}")
    if len(all_files) == 0:
        print(f"⚠️  Skipping CWE-{cwe_number} - no files found")
        return

    selected_files = load_or_create_selection(cwe_number, all_files)
    num_to_select = len(selected_files)

    cwe_output_dir = OUTPUT_DIR / f"CWE{cwe_number}"
    cwe_output_dir.mkdir(parents=True, exist_ok=True)

    results = {
        "cwe": cwe_number,
        "timestamp": datetime.now().isoformat(),
        "total_files": len(all_files),
        "selected_files": num_to_select,
        "files": []
    }

    for idx, file_path in enumerate(selected_files, 1):
        print(f"\n📄 [{idx}/{num_to_select}] Processing: {file_path.name}")

        file_info = extract_file_info(file_path)
        if not file_info:
            continue

        print(f"   Good functions: {', '.join(file_info['good_functions']) if file_info['good_functions'] else 'None'}")
        print(f"   Bad functions:  {', '.join(file_info['bad_functions']) if file_info['bad_functions'] else 'None'}")

        # Compact bodies (helpful for JSON/text prompts)
        bad_bodies = [
            _find_function_body_across_chain(file_path, file_info["content"], n, 900)
            for n in file_info["bad_functions"]
        ]
        good_bodies = [
            _find_function_body_across_chain(file_path, file_info["content"], n, 900)
            for n in file_info["good_functions"]
        ]
        bad_bodies = [b for b in bad_bodies if b][:2]
        good_bodies = [g for g in good_bodies if g][:2]

        # (A) Try C harness first
        c_prompt, syms = build_c_harness_prompt(file_info)
        if c_prompt and syms:
            good_sym, bad_sym = syms
            print("   🤖 Asking model for C harness ...")
            c_code = call_model(provider, model_name, c_prompt, mode="c")
            if c_code and "main(" in c_code and good_sym in c_code and bad_sym in c_code:
                out_c = cwe_output_dir / f"{file_path.stem}_harness.c"
                out_c.write_text(c_code)
                results["files"].append({
                    "file_path": str(file_path.relative_to(JULIET_BASE.parent)),
                    "file_name": file_path.name,
                    "good_functions": file_info["good_functions"],
                    "bad_functions": file_info["bad_functions"],
                    "generated": "c_harness",
                    "good_sym": good_sym,
                    "bad_sym": bad_sym,
                    "provider": provider,
                    "model": model_name,
                })
                print(f"   💾 Saved C harness: {out_c}")
                time.sleep(0.6)
                continue
            else:
                print("   ⚠️ C harness generation failed, falling back...")

        # (B) Strict JSON
        strict_prompt = generate_test_cases_prompt(
            file_info, cwe_number, bad_bodies=bad_bodies, good_bodies=good_bodies
        )
        print("   🤖 Calling model (strict JSON) ...")
        test_cases = call_model(provider, model_name, strict_prompt, mode="json", simple=False)
        if test_cases and "test_cases" in test_cases:
            out_json = cwe_output_dir / f"{file_path.stem}_tests.json"
            file_result = {
                "file_path": str(file_path.relative_to(JULIET_BASE.parent)),
                "file_name": file_path.name,
                "good_functions": file_info["good_functions"],
                "bad_functions": file_info["bad_functions"],
                "test_cases": test_cases["test_cases"],
                "provider": provider,
                "model": model_name,
            }
            out_json.write_text(json.dumps(file_result, indent=2))
            results["files"].append(file_result)
            print(f"   💾 Saved JSON tests: {out_json}")
            time.sleep(0.6)
            continue

        # (C) Simple text fallback
        print("   ⚠️ Strict JSON failed → trying simple text mode...")
        simple_prompt = build_minimal_prompt(file_info, cwe_number, bad_bodies=bad_bodies, good_bodies=good_bodies)
        simple_text = call_model(provider, model_name, simple_prompt, mode="json", simple=True)
        if simple_text:
            out_txt = cwe_output_dir / f"{file_path.stem}_tests.txt"
            out_txt.write_text(simple_text)
            results["files"].append({
                "file_path": str(file_path.relative_to(JULIET_BASE.parent)),
                "file_name": file_path.name,
                "good_functions": file_info["good_functions"],
                "bad_functions": file_info["bad_functions"],
                "generated": "text_ideas",
                "provider": provider,
                "model": model_name,
            })
            print(f"   💾 Saved text ideas: {out_txt}")
        else:
            print("   ❌ Failed in all modes")
            results["files"].append({
                "file_path": str(file_path.relative_to(JULIET_BASE.parent)),
                "file_name": file_path.name,
                "error": "Failed to generate (c/json/text)",
                "provider": provider,
                "model": model_name,
            })

        time.sleep(0.6)

    summary_file = cwe_output_dir / f"CWE{cwe_number}_summary.json"
    with open(summary_file, "w") as f:
        json.dump(results, f, indent=2)

    print(f"\n✅ Completed CWE-{cwe_number}")
    ok = len([f for f in results["files"] if "error" not in f])
    print(f"   📊 Successfully produced artifacts for: {ok}/{num_to_select} files")
    print(f"   💾 Summary saved to: {summary_file}")


def parse_args():
    ap = argparse.ArgumentParser(description="Generate LLM-authored test artifacts for a selected Juliet CWE.")
    ap.add_argument("--cwe", type=int, required=True, help=f"One of: {ALLOWED_CWES}")
    ap.add_argument("--provider", type=str, default=DEFAULT_PROVIDER, choices=["google", "openai"], help="LLM provider")
    ap.add_argument("--model", type=str, default=None, help="Override model name")
    return ap.parse_args()


def main():
    args = parse_args()
    cwe = args.cwe
    provider = args.provider
    model_name = args.model or (DEFAULT_GOOGLE_MODEL if provider == "google" else DEFAULT_OPENAI_MODEL)

    if cwe not in ALLOWED_CWES:
        print(f"❌ Invalid CWE: {cwe}. Must be one of: {ALLOWED_CWES}")
        sys.exit(1)

    print("="*70)
    print("LLM Test Artifact Generator for Juliet")
    print("="*70)
    print(f"Provider: {provider}")
    print(f"Model:    {model_name}")
    print(f"Selected CWE: {cwe}")
    print(f"Files per CWE (first run): {FILES_PER_CWE}")
    print(f"Output directory: {OUTPUT_DIR}")
    print("="*70)

    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)

    if not JULIET_BASE.exists():
        print(f"\n❌ Juliet test suite not found at: {JULIET_BASE}")
        print("   Please ensure the Juliet dataset is set up correctly")
        sys.exit(1)

    print(f"\n✅ Juliet base found: {JULIET_BASE}")

    try:
        process_cwe(cwe, provider, model_name)
    except KeyboardInterrupt:
        print("\n\n⚠️  Interrupted by user")
    except Exception as e:
        print(f"\n❌ Error processing CWE-{cwe}: {e}")
        import traceback
        traceback.print_exc()

    print("\n" + "="*70)
    print("🎉 Generation Complete")
    print("="*70)
    print(f"📁 Results saved in: {OUTPUT_DIR}/")
    print(f"ℹ️ Stable selection file: {OUTPUT_DIR}/CWE{cwe}/selected_files.json")
    print("="*70)


if __name__ == "__main__":
    main()
