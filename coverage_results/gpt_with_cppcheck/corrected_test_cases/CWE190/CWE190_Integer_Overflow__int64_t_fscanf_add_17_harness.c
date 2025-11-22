// ```c
#include <inttypes.h>
#include <stdio.h>

void CWE190_Integer_Overflow__int64_t_fscanf_add_17_bad(void);
void CWE190_Integer_Overflow__int64_t_fscanf_add_17_good(void);

void run_bad(void) {
    printf("Running Bad Test...\n");
    // Simulating input to trigger the integer overflow condition:
    int64_t data;
    data = INT64_MAX; // Set data to a large value to force an overflow when adding 1
    printf("%" PRId64 "\n", data);
    // Directly call the bad function to simulate the condition
    CWE190_Integer_Overflow__int64_t_fscanf_add_17_bad();
}

void run_good(void) {
    printf("Running Good Test...\n");
    // Directly call the good function that should handle edge cases
    CWE190_Integer_Overflow__int64_t_fscanf_add_17_good();
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
// ```