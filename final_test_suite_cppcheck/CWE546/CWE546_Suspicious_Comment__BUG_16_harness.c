```c
#include <stdio.h>

void CWE546_Suspicious_Comment__BUG_16_bad(void);
void CWE546_Suspicious_Comment__BUG_16_good(void);

void run_bad(void) {
    printf("Running bad function...\n");
    CWE546_Suspicious_Comment__BUG_16_bad();
}

void run_good(void) {
    printf("Running good function...\n");
    CWE546_Suspicious_Comment__BUG_16_good();
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