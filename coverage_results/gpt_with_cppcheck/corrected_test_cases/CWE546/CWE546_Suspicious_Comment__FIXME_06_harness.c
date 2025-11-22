// ```c
#include <stdio.h>

void CWE546_Suspicious_Comment__FIXME_06_bad(void);
void CWE546_Suspicious_Comment__FIXME_06_good(void);

void run_bad(void) {
    printf("Running bad case...\n");
    CWE546_Suspicious_Comment__FIXME_06_bad();
    printf("Finished running bad case.\n");
}

void run_good(void) {
    printf("Running good case...\n");
    CWE546_Suspicious_Comment__FIXME_06_good();
    printf("Finished running good case.\n");
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