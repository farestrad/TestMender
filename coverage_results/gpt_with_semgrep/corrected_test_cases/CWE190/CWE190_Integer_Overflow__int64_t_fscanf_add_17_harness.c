// ```c
#include <stdio.h>
#include <inttypes.h>

void CWE190_Integer_Overflow__int64_t_fscanf_add_17_bad(void);
void CWE190_Integer_Overflow__int64_t_fscanf_add_17_good(void);

void run_bad(void) {
    printf("Executing BAD test case...\n");
    CWE190_Integer_Overflow__int64_t_fscanf_add_17_bad();
}

void run_good(void) {
    printf("Executing GOOD test cases...\n");
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