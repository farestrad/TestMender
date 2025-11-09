#!/usr/bin/env python3
import csv, re, sys, pathlib

ROOT = pathlib.Path(".")
JULIET_CANDIDATES = [
    ROOT / "data" / "juliet" / "testcases",
    ROOT / "data" / "juliet-c" / "testcases",
    ROOT / "data" / "Juliet_Test_Suite_C" / "testcases",
]
JULIET_ROOT = next((p for p in JULIET_CANDIDATES if p.exists()), None)

N_PER_CWE = 3  # cases per CWE

# Pilot CWEs -> folder numbers present in your tree
CWE_PATTERNS = {
    "CWE-121": [121],                 # Stack overflow
    "CWE-122": [122],                 # Heap overflow
    "CWE-190": [190, 191],            # Integer overflow/underflow
    "CWE-22":  [23, 36],              # Relative/Absolute Path Traversal
    "CWE-362": [364, 366, 367],       # Race/TOCTOU families
    "CWE-327": [327, 328],            # Weak crypto/hash
    "CWE-256": [256, 259],            # Plaintext/Hard-coded password
    "CWE-338": [338],                 # Weak PRNG
}

ORACLES = {
    "CWE-121": ("asan_crash","address-sanitizer","-O0 -g -fsanitize=address -fno-omit-frame-pointer"),
    "CWE-122": ("asan_crash","address-sanitizer","-O0 -g -fsanitize=address -fno-omit-frame-pointer"),
    "CWE-190": ("ubsan_overflow","signed-integer-overflow","-O0 -g -fsanitize=undefined"),
    "CWE-22":  ("static_hit","TRAVERSAL",""),
    "CWE-362": ("exit_code","1",""),
    "CWE-327": ("static_hit","WEAK_CRYPTO",""),
    "CWE-256": ("static_hit","PLAINTEXT_SECRET",""),
    "CWE-338": ("static_hit","WEAK_RNG",""),
}

HEADER = ["pilot_id","cwe","case_id","src_file","good_func","bad_func",
          "build_flags","run_args","oracle_type","oracle_value","notes"]

good_re = re.compile(r"\b(\w*good\w*)\s*\(", re.IGNORECASE)
bad_re  = re.compile(r"\b(\w*bad\w*)\s*\(",  re.IGNORECASE)

def find_funcs(text: str):
    good = sorted(set(good_re.findall(text)))[:1]
    bad  = sorted(set(bad_re.findall(text)))[:1]
    return (good[0] if good else None), (bad[0] if bad else None)

def dirs_for_cwe(pilot_cwe: str):
    """Return dirs whose name contains an exact CWE number (e.g., 36 matches CWE36, not CWE364)."""
    nums = CWE_PATTERNS[pilot_cwe]
    hits = []
    if JULIET_ROOT is None:
        return hits
    for d in JULIET_ROOT.iterdir():
        if not d.is_dir():
            continue
        name = d.name.lower()
        for n in nums:
            # match 'cwe<n>' with no extra digit immediately after (avoid 36 -> 364)
            if re.search(rf"\bcwe{n}(?!\d)", name):
                hits.append(d)
                break
    return sorted(hits)


def is_candidate_file(p: pathlib.Path) -> bool:
    # Skip build/support dirs
    low_parts = {q.lower() for q in p.parts}
    if "cmakefiles" in low_parts or "testcasesupport" in low_parts:
        return False
    # Only Juliet case files (start with CWE, end with .c)
    return p.name.startswith("CWE") and p.suffix == ".c"

def pick_c_files(dirpath: pathlib.Path, n: int):
    candidates = []
    # Prefer sNN subdirs if present
    s_dirs = sorted([d for d in dirpath.glob("s*") if d.is_dir()])
    search_roots = s_dirs if s_dirs else [dirpath]
    for root in search_roots:
        for f in sorted(root.rglob("*.c")):
            if is_candidate_file(f):
                candidates.append(f)
    return candidates[:n]

def main():
    out = ROOT / "config" / "pilot_manifest.csv"
    out.parent.mkdir(parents=True, exist_ok=True)

    if JULIET_ROOT is None:
        print("[err] Could not find a Juliet 'testcases' directory.")
        for p in JULIET_CANDIDATES:
            print(f" - {p}")
        print(f"[ok] wrote {out} with 0 rows")
        return 0

    print(f"[info] Using JULIET_ROOT = {JULIET_ROOT}")

    rows = []
    pid = 1
    for pilot_cwe in CWE_PATTERNS:
        cwe_dirs = dirs_for_cwe(pilot_cwe)
        if not cwe_dirs:
            print(f"[warn] No directories found for {pilot_cwe} under {JULIET_ROOT}")
        collected = 0
        for cwe_dir in cwe_dirs:
            for f in pick_c_files(cwe_dir, 9999):
                if collected >= N_PER_CWE:
                    break
                try:
                    text = f.read_text(errors="ignore")
                except Exception:
                    text = ""
                good, bad = find_funcs(text)
                if not (good and bad):
                    continue  # must have both
                oracle_type, oracle_value, build_flags = ORACLES.get(pilot_cwe, ("static_hit","", ""))
                case_id = f.stem
                pilot_id = f"pilot_v1_{pid:04d}"
                rel = str(f.relative_to(ROOT))
                rows.append([pilot_id, pilot_cwe, case_id, rel, good, bad,
                             build_flags, "", oracle_type, oracle_value, ""])
                pid += 1
                collected += 1
            if collected >= N_PER_CWE:
                break

    with out.open("w", newline="") as fh:
        w = csv.writer(fh)
        w.writerow(HEADER)
        w.writerows(rows)

    print(f"[ok] wrote {out} with {len(rows)} rows")
    return 0

if __name__ == "__main__":
    sys.exit(main())
