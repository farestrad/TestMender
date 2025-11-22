// ```c
#include <stdio.h>

// Prototypes for the Juliet entry functions
#ifndef OMITBAD
void CWE546_Suspicious_Comment__BUG_09_bad(void);
#endif /* OMITBAD */
#ifndef OMITGOOD
void CWE546_Suspicious_Comment__BUG_09_good(void);
#endif /* OMITGOOD */

void run_bad(void) {
    printf("Running bad case...\n");
    CWE546_Suspicious_Comment__BUG_09_bad();
}

void run_good(void) {
    printf("Running good case...\n");
    CWE546_Suspicious_Comment__BUG_09_good();
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