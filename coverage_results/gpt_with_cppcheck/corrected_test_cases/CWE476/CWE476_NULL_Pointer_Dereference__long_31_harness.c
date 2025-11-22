// ```c
#include <stdio.h>

void CWE476_NULL_Pointer_Dereference__long_31_bad(void);
void CWE476_NULL_Pointer_Dereference__long_31_good(void);

void run_bad(void) {
    printf("Running BAD test case...\n");
    CWE476_NULL_Pointer_Dereference__long_31_bad();
}

void run_good(void) {
    printf("Running GOOD test case...\n");
    CWE476_NULL_Pointer_Dereference__long_31_good();
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