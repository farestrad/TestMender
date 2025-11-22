// ```c
#include <stdio.h>

void CWE476_NULL_Pointer_Dereference__deref_after_check_01_bad(void);
void CWE476_NULL_Pointer_Dereference__deref_after_check_01_good(void);

void run_bad(void) {
    printf("Running bad case...\n");
    CWE476_NULL_Pointer_Dereference__deref_after_check_01_bad();
}

void run_good(void) {
    printf("Running good case...\n");
    CWE476_NULL_Pointer_Dereference__deref_after_check_01_good();
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