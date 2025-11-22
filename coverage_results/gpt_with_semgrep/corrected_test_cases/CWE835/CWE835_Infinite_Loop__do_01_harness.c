// ```c
#include "std_testcase.h"

#ifndef OMITBAD
void CWE835_Infinite_Loop__do_01_bad(void);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE835_Infinite_Loop__do_01_good(void);
#endif /* OMITGOOD */

void run_bad(void) {
    printf("Running BAD test case...\n");
    CWE835_Infinite_Loop__do_01_bad();
    printf("Finished BAD test case.\n");
}

void run_good(void) {
    printf("Running GOOD test case...\n");
    CWE835_Infinite_Loop__do_01_good();
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