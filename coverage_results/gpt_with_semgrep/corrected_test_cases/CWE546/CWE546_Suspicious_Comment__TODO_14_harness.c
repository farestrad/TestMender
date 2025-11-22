// ```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void CWE546_Suspicious_Comment__TODO_14_bad(void);
void CWE546_Suspicious_Comment__TODO_14_good(void);

void run_bad(void) {
    printf("Running bad case...\n");
    CWE546_Suspicious_Comment__TODO_14_bad();
}

void run_good(void) {
    printf("Running good case...\n");
    CWE546_Suspicious_Comment__TODO_14_good();
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