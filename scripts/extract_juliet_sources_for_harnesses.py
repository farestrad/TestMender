#!/usr/bin/env python3
import argparse
import shutil
from pathlib import Path
from typing import List, Optional


def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(
        description="Extract Juliet source files for all harnesses we generate tests for."
    )
    p.add_argument(
        "--juliet-root",
        type=Path,
        default=Path("data/juliet/testcases"),
        help="Root of the Juliet testcases tree (default: data/juliet/testcases)",
    )
    p.add_argument(
        "--harness-root",
        type=Path,
        action="append",
        required=True,
        help=(
            "Root directory where harnesses live. "
            "Can be passed multiple times, e.g. "
            "--harness-root gpt_cppcheck_generated_test "
            "--harness-root improved_tests/cppcheck"
        ),
    )
    p.add_argument(
        "--out-root",
        type=Path,
        default=Path("analysis_results/extracted_juliet_sources"),
        help="Where to copy the Juliet sources (default: analysis_results/extracted_juliet_sources)",
    )
    return p.parse_args()


def extract_cwe_from_stem(stem: str) -> Optional[str]:
    """
    Extract the CWE number from a stem like:
    'CWE121_Stack_Based_Buffer_Overflow__CWE193_...'

    Returns e.g. '121' or None if it can't parse.
    """
    if not stem.startswith("CWE"):
        return None
    # Format: CWE<NUM>_...
    # Find first underscore
    try:
        after = stem[3:]
        num = ""
        for ch in after:
            if ch.isdigit():
                num += ch
            else:
                break
        return num if num else None
    except Exception:
        return None


def find_juliet_source(juliet_root: Path, stem: str) -> Optional[Path]:
    """
    Search for <stem>.c under juliet_root. Returns a single path if exactly one match,
    None if none or ambiguous (multiple).
    """
    matches = list(juliet_root.rglob(f"{stem}.c"))
    if len(matches) == 1:
        return matches[0]
    elif len(matches) == 0:
        print(f"  [WARN] No Juliet source found for stem {stem}")
        return None
    else:
        print(f"  [WARN] Multiple Juliet sources found for stem {stem}:")
        for m in matches:
            print(f"         - {m}")
        return None


def gather_harness_files(harness_roots: List[Path]) -> List[Path]:
    harness_files: List[Path] = []
    for root in harness_roots:
        if not root.exists():
            print(f"[WARN] harness root does not exist: {root}")
            continue
        print(f"[INFO] Scanning harness root: {root}")
        harness_files.extend(root.rglob("*_harness.c"))
        harness_files.extend(root.rglob("*_harness_improved.c"))
    harness_files = sorted(set(harness_files))
    print(f"[INFO] Found {len(harness_files)} harness file(s) total.")
    return harness_files


def main() -> None:
    args = parse_args()
    juliet_root: Path = args.juliet_root
    harness_roots: List[Path] = args.harness_root
    out_root: Path = args.out_root

    print(f"[CFG] Juliet root       : {juliet_root}")
    print(f"[CFG] Harness root(s)   : {', '.join(str(r) for r in harness_roots)}")
    print(f"[CFG] Output root       : {out_root}")

    if not juliet_root.exists():
        raise SystemExit(f"[FATAL] Juliet root does not exist: {juliet_root}")

    out_root.mkdir(parents=True, exist_ok=True)

    harness_files = gather_harness_files(harness_roots)

    copied = 0
    seen_stems = set()

    for harness in harness_files:
        stem = harness.name
        # strip suffixes: _harness.c or _harness_improved.c
        if stem.endswith("_harness_improved.c"):
            stem = stem[: -len("_harness_improved.c")]
        elif stem.endswith("_harness.c"):
            stem = stem[: -len("_harness.c")]
        else:
            # shouldn't happen due to glob pattern
            continue

        if stem in seen_stems:
            continue
        seen_stems.add(stem)

        cwe_num = extract_cwe_from_stem(stem)
        if cwe_num is None:
            print(f"[WARN] Could not extract CWE from stem: {stem}")
            cwe_dir = out_root / "unknown_cwe"
        else:
            cwe_dir = out_root / f"CWE{cwe_num}"

        src = find_juliet_source(juliet_root, stem)
        if src is None:
            continue

        cwe_dir.mkdir(parents=True, exist_ok=True)
        dest = cwe_dir / src.name

        # If multiple harness roots point at the same Juliet file, we only copy once.
        if dest.exists():
            print(f"[SKIP] Already copied: {dest}")
            continue

        shutil.copy2(src, dest)
        copied += 1
        print(f"[COPY] {src} -> {dest}")

    print(f"\n[SUMMARY] Copied {copied} Juliet source file(s) into {out_root}")


if __name__ == "__main__":
    main()
