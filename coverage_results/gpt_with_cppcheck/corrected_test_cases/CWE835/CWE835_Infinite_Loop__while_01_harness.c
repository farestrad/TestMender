// ```c
#include "std_testcase.h"
#include <stdio.h>

void CWE835_Infinite_Loop__while_01_bad(void);
void CWE835_Infinite_Loop__while_01_good(void);

void run_bad(void) {
    printf("Running BAD function...\n");
    CWE835_Infinite_Loop__while_01_bad();
    printf("Finished BAD function.\n");
}

void run_good(void) {
    printf("Running GOOD function...\n");
    CWE835_Infinite_Loop__while_01_good();
    printf("Finished GOOD function.\n");
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