```c
#include <stdio.h>

#ifndef OMITBAD
void CWE546_Suspicious_Comment__BUG_02_bad(void);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE546_Suspicious_Comment__BUG_02_good(void);
#endif /* OMITGOOD */

void run_bad(void) {
    printf("Running bad test case...\n");
    CWE546_Suspicious_Comment__BUG_02_bad();
}

void run_good(void) {
    printf("Running good test case...\n");
    CWE546_Suspicious_Comment__BUG_02_good();
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```