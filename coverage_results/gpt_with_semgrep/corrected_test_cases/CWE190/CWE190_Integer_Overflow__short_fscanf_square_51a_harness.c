// ```c
#include <stdio.h>

void CWE190_Integer_Overflow__short_fscanf_square_51_bad(void);
void CWE190_Integer_Overflow__short_fscanf_square_51_good(void);

void run_bad(void) {
    printf("Running BAD test case...\n");
    CWE190_Integer_Overflow__short_fscanf_square_51_bad();
}

void run_good(void) {
    printf("Running GOOD test cases...\n");
    CWE190_Integer_Overflow__short_fscanf_square_51_good();
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