```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void CWE546_Suspicious_Comment__HACK_02_bad(void);
void CWE546_Suspicious_Comment__HACK_02_good(void);

void run_bad(void) {
    printf("Running bad test case...\n");
    CWE546_Suspicious_Comment__HACK_02_bad();
    printf("Finished bad test case.\n");
}

void run_good(void) {
    printf("Running good test case...\n");
    CWE546_Suspicious_Comment__HACK_02_good();
    printf("Finished good test case.\n");
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