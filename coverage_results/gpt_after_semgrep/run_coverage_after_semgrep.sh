#!/bin/bash

# --- CONFIGURATION ---
ROOT_SRC="source_files"
ROOT_TEST="corrected_test_cases"
OUT_DIR="output_files"
INCLUDE_DIR="testcasesupport"
# ---------------------

# Check arguments
if [ $# -eq 0 ]; then
    echo "Usage: $0 CWE_ID1 CWE_ID2 ..."
    exit 1
fi

# Ensure output directory and dependency exist
mkdir -p "$OUT_DIR"
if [ ! -f "$OUT_DIR/io.o" ]; then
    echo "ERROR: '$OUT_DIR/io.o' is missing. Please compile io.c first."
    exit 1
fi

# Get absolute path for temporary data handling
CURRENT_DIR=$(pwd)

# --- MAIN LOOP (Per CWE ID) ---
for CWE_ID in "$@"; do
    
    CWE_ID=${CWE_ID%/} 
    SRC_SUBDIR="$ROOT_SRC/$CWE_ID"
    TEST_SUBDIR="$ROOT_TEST/$CWE_ID"
    RESULT_FILE="$CURRENT_DIR/${CWE_ID}_coverage_results_afterSemgrep.txt"
    TEMP_MATH_FILE="$CURRENT_DIR/${CWE_ID}_percentages.tmp"

    if [[ ! -d "$SRC_SUBDIR" || ! -d "$TEST_SUBDIR" ]]; then
        echo "Skipping $CWE_ID: Folders not found."
        continue
    fi

    # Clear files
    > "$RESULT_FILE"
    rm -f "$TEMP_MATH_FILE"

    echo "Processing $CWE_ID..."

    # --- INNER LOOP (Per File) ---
    for harness_path in "$TEST_SUBDIR"/*_harness.c; do
        [ -e "$harness_path" ] || continue

        harness_filename=$(basename "$harness_path")
        base_name=${harness_filename%_harness.c}
        source_path="$SRC_SUBDIR/${base_name}.c"

        if [ -f "$source_path" ]; then
            
            # Define artifact paths
            obj_source="$OUT_DIR/${base_name}.o"
            obj_harness="$OUT_DIR/${base_name}_harness.o"
            runner_exec="$OUT_DIR/runner.exe"

            # 1. Compile Source
            gcc --coverage -c -I"$INCLUDE_DIR" "$source_path" -o "$obj_source" > /dev/null 2>&1
            
            # 2. Compile Harness
            gcc --coverage -c -I"$INCLUDE_DIR" "$harness_path" -o "$obj_harness" > /dev/null 2>&1

            # 3. Link
            if [ -f "$obj_source" ] && [ -f "$obj_harness" ]; then
                gcc --coverage -I"$INCLUDE_DIR" "$obj_source" "$obj_harness" "$OUT_DIR/io.o" -o "$runner_exec" > /dev/null 2>&1
            fi

            # 4. Run Executable (FIXED: Added timeout and input redirection)
            if [ -f "$runner_exec" ]; then
                # timeout 2s: Kills the process if it runs longer than 2 seconds
                # < /dev/null: If the C code has a scanf(), it immediately gets EOF instead of hanging
                timeout 2s "$runner_exec" < /dev/null > /dev/null 2>&1
            fi

            # 5. GCOV & Extract Data
            (
                cd "$OUT_DIR" || exit
                
                raw_line=$(gcov "$harness_filename" 2>/dev/null | grep "Lines executed:")
                percent_str=$(echo "$raw_line" | grep -oE "[0-9]+(\.[0-9]+)?%")

                if [ ! -z "$percent_str" ]; then
                    echo "Pair: $base_name" >> "$RESULT_FILE"
                    echo "Lines executed:$percent_str" >> "$RESULT_FILE"
                    echo "" >> "$RESULT_FILE"
                    echo "${percent_str%\%}" >> "$TEMP_MATH_FILE"
                fi

                # Cleanup
                rm -f "${base_name}"*.gcda "${base_name}"*.gcno
            )
        fi
    done

    # --- CALCULATE AVERAGE ---
    if [ -f "$TEMP_MATH_FILE" ]; then
        AVERAGE=$(awk '{ sum += $1; n++ } END { if (n > 0) printf "%.2f", sum / n; }' "$TEMP_MATH_FILE")
        
        echo "----------------------------------------" >> "$RESULT_FILE"
        COUNT=$(wc -l < "$TEMP_MATH_FILE")
        echo "Total Pairs: $COUNT" >> "$RESULT_FILE"
        echo "Average Coverage: $AVERAGE%" >> "$RESULT_FILE"
        echo "----------------------------------------" >> "$RESULT_FILE"
        
        echo "  > Completed $CWE_ID. Average: $AVERAGE%"
        rm "$TEMP_MATH_FILE"
    else
        echo "  > Completed $CWE_ID. No valid coverage data found."
    fi

done