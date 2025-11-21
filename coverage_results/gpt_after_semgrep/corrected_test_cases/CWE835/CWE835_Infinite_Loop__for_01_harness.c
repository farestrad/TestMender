// ```c
#include "std_testcase.h"

#ifndef OMITBAD
void CWE835_Infinite_Loop__for_01_bad(void);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE835_Infinite_Loop__for_01_good(void);
#endif /* OMITGOOD */

void run_bad(void) {
    printf("Running bad case...\n");
    CWE835_Infinite_Loop__for_01_bad();
}

void run_good(void) {
    printf("Running good case...\n");
    CWE835_Infinite_Loop__for_01_good();
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