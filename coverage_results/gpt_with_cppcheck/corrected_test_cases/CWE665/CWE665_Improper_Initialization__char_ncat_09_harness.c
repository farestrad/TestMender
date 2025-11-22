// ```c
#include <stdio.h>

void CWE665_Improper_Initialization__char_ncat_09_bad(void);
void CWE665_Improper_Initialization__char_ncat_09_good(void);

void run_bad(void) {
    printf("Running bad test...\n");
    CWE665_Improper_Initialization__char_ncat_09_bad();
}

void run_good(void) {
    printf("Running good test...\n");
    CWE665_Improper_Initialization__char_ncat_09_good();
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