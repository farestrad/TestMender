// ```c
#include <stdio.h>

/* Declare prototypes for the Juliet functions */
void CWE546_Suspicious_Comment__HACK_15_bad(void);
void CWE546_Suspicious_Comment__HACK_15_good(void);

void run_bad(void) {
    printf("Running bad() - will execute code with suspicious comment...\n");
    CWE546_Suspicious_Comment__HACK_15_bad();
}

void run_good(void) {
    printf("Running good() - will execute code with safe comments...\n");
    CWE546_Suspicious_Comment__HACK_15_good();
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