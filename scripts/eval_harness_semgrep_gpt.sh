#!/usr/bin/env bash
set -euo pipefail

# Usage:
#   ./scripts/eval_harness_semgrep_gpt.sh 121
#
# Assumes harnesses are in: gpt_sem_generated_test/CWE<NUMBER>

CWE="${1:-121}"  # default to 121 if not provided

./scripts/run_harness_batch_text.sh "$CWE" \
  --mode=both \
  --out="harness_results/segrep_gpt/cwe${CWE}_results.txt" \
  --timeout=15
