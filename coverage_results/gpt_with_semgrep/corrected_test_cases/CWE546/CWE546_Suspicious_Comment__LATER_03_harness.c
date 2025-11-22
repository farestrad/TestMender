// ```c
#include <stdio.h>

#ifndef OMITBAD
void CWE546_Suspicious_Comment__LATER_03_bad(void);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE546_Suspicious_Comment__LATER_03_good(void);
#endif /* OMITGOOD */

void run_bad(void) {
    printf("Running bad case...\n");
    CWE546_Suspicious_Comment__LATER_03_bad();
    printf("Finished running bad case.\n");
}

void run_good(void) {
    printf("Running good case...\n");
    CWE546_Suspicious_Comment__LATER_03_good();
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