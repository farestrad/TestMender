#!/usr/bin/env python3
"""Fetch pinned Juliet sources and compile selected CWEs; never execute tests."""
import argparse
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "data/juliet"
URL = "https://github.com/arichardson/juliet-test-suite-c.git"
REVISION = "f88433e3443648a17671398797a04ea1f8e1a274"


def run(*args):
    subprocess.run(args, check=True)


def git_output(*args):
    return subprocess.check_output(["git", "-C", str(SOURCE), *args], text=True).strip()


def setup():
    if SOURCE.exists() and any(SOURCE.iterdir()):
        # Never reset, pull, or replace an existing dataset.
        if git_output("rev-parse", "--show-toplevel") != str(SOURCE.resolve()):
            raise RuntimeError("Existing data/juliet is not a standalone Git checkout")
        if git_output("rev-parse", "HEAD") != REVISION:
            raise RuntimeError("Existing Juliet revision differs from the pin; preserve it and use a fresh clone")
        if git_output("diff", "HEAD", "--name-only"):
            raise RuntimeError("Juliet has tracked edits; preserve them before rebuilding")
    else:
        SOURCE.parent.mkdir(parents=True, exist_ok=True)
        # Failed downloads leave no partial dataset behind.
        with tempfile.TemporaryDirectory(prefix=".juliet-fetch-", dir=SOURCE.parent) as temporary:
            checkout = Path(temporary) / "source"
            run("git", "init", str(checkout))
            run("git", "-C", str(checkout), "remote", "add", "origin", URL)
            run("git", "-C", str(checkout), "fetch", "--depth=1", "origin", REVISION)
            run("git", "-C", str(checkout), "checkout", "--detach", REVISION)
            if SOURCE.exists():
                SOURCE.rmdir()  # Only removes an empty directory.
            checkout.rename(SOURCE)
    if git_output("rev-parse", "HEAD") != REVISION:
        raise RuntimeError("Juliet revision verification failed")
    print("Juliet source verified:", REVISION, flush=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("cwes", nargs="*", help="CWE numbers, e.g. 546 121")
    parser.add_argument("--all", action="store_true", help="Build every category (platform failures are possible)")
    parser.add_argument("--setup-only", action="store_true")
    parser.add_argument("--jobs", type=int, default=2)
    args = parser.parse_args()
    if args.jobs < 1 or any(not re.fullmatch(r"[1-9][0-9]*", c) for c in args.cwes):
        parser.error("Use positive integers for CWE numbers and --jobs")
    if not args.setup_only and not (args.all or args.cwes):
        parser.error("Select CWE numbers or --all")
    if args.all and args.cwes:
        parser.error("Select either CWE numbers or --all")
    setup()
    if args.setup_only:
        return
    categories = sorted(p for p in (SOURCE / "testcases").iterdir()
                        if p.is_dir() and re.match(r"^CWE[0-9]+_", p.name))
    selected = categories if args.all else [p for p in categories if p.name.split("_", 1)[0][3:] in args.cwes]
    found = {p.name.split("_", 1)[0][3:] for p in selected}
    if not args.all and set(args.cwes) - found:
        parser.error("Unknown CWE numbers: " + ", ".join(sorted(set(args.cwes) - found)))
    template = (SOURCE / "CMakeLists.txt").read_bytes()
    failed = []
    for category in selected:
        config = category / "CMakeLists.txt"
        if config.exists() and config.read_bytes() != template:
            raise RuntimeError("Refusing to overwrite customized " + str(config))
        if not config.exists():
            shutil.copyfile(SOURCE / "CMakeLists.txt", config)
        build = ROOT / "build/juliet" / category.name
        try:
            run("cmake", "-S", str(category), "-B", str(build),
                "-DCMAKE_POLICY_VERSION_MINIMUM=3.5", "-DOUTPUT_DIR:STRING=bin")
            run("cmake", "--build", str(build), "--parallel", str(args.jobs))
        except subprocess.CalledProcessError:
            failed.append(category.name)
    if failed:
        raise RuntimeError("Build failed for: " + ", ".join(failed))
    print("Build complete. Binaries: data/juliet/bin/. No test programs were executed.")


if __name__ == "__main__":
    try:
        main()
    except (RuntimeError, subprocess.CalledProcessError) as error:
        print("Error:", error, file=sys.stderr)
        sys.exit(1)
