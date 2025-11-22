// ```c
#include <stdio.h>
#include <stdlib.h>

void CWE835_Infinite_Loop__while_true_01_bad(void);
void CWE835_Infinite_Loop__while_true_01_good(void);

void run_bad(void) {
    printf("Running bad() function...\n");
    CWE835_Infinite_Loop__while_true_01_bad();
    printf("Finished running bad() function.\n");
}

void run_good(void) {
    printf("Running good() function...\n");
    CWE835_Infinite_Loop__while_true_01_good();
    printf("Finished running good() function.\n");
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