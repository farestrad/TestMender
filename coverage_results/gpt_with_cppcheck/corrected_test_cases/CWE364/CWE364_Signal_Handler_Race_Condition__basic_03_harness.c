// ```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

typedef struct _structSigAtomic
{
    sig_atomic_t val;
} structSigAtomic;

// Declare prototypes for the Juliet entry functions
void CWE364_Signal_Handler_Race_Condition__basic_03_bad(void);
void CWE364_Signal_Handler_Race_Condition__basic_03_good(void);

void run_bad(void) {
    printf("Running BAD test case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_03_bad();
    printf("Finished BAD test case.\n");
}

void run_good(void) {
    printf("Running GOOD test case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_03_good();
    printf("Finished GOOD test case.\n");
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