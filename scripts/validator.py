import subprocess
import os
import shutil
import json

def compile_harness(harness_path):
    exe = harness_path.replace(".c", "")
    cmd = ["gcc", harness_path, "-o", exe]
    result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return exe, result.returncode == 0, result.stderr.decode()

def run_binary(exe_path):
    result = subprocess.run([exe_path], stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE, timeout=5)
    return result.returncode, result.stdout.decode(), result.stderr.decode()

def validate(harness_path, metadata=None):
    exe, ok, err = compile_harness(harness_path)
    if not ok:
        return {
            "status": "reject",
            "reason": "compile_error",
            "details": err,
            "good_exit_code": None,
            "bad_exit_code": None,
        }

    good_code, good_out, good_err = run_binary(exe)
    bad_code, bad_out, bad_err = run_binary(exe)

    # GOOD should pass (0)
    if good_code != 0:
        return {
            "status": "reject",
            "reason": "good_failed",
            "details": good_err,
            "good_exit_code": good_code,
            "bad_exit_code": bad_code,
        }

    # BAD should fail (!= 0)
    if bad_code == 0:
        return {
            "status": "reject",
            "reason": "bad_passed",
            "details": bad_out,
            "good_exit_code": good_code,
            "bad_exit_code": bad_code,
        }

    return {
        "status": "accept",
        "reason": "validated",
        "details": "",
        "good_exit_code": good_code,
        "bad_exit_code": bad_code,
    }
