// ```c
#include "std_testcase.h"

#ifndef OMITBAD
void CWE190_Integer_Overflow__int_rand_preinc_15_bad(void);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE190_Integer_Overflow__int_rand_preinc_15_good(void);
#endif /* OMITGOOD */

void run_bad(void) {
    printf("Running bad case...\n");
    CWE190_Integer_Overflow__int_rand_preinc_15_bad();
    printf("Finished running bad case.\n");
}

void run_good(void) {
    printf("Running good case...\n");
    CWE190_Integer_Overflow__int_rand_preinc_15_good();
    printf("Finished running good case.\n");
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