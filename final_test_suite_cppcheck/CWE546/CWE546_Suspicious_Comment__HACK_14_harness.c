```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern void CWE546_Suspicious_Comment__HACK_14_bad(void);
extern void CWE546_Suspicious_Comment__HACK_14_good(void);

void run_bad(void) {
    printf("Running bad example...\n");
    CWE546_Suspicious_Comment__HACK_14_bad();
    printf("Finished running bad example.\n");
}

void run_good(void) {
    printf("Running good example...\n");
    CWE546_Suspicious_Comment__HACK_14_good();
    printf("Finished running good example.\n");
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