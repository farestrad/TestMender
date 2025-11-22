// ```c
#include "std_testcase.h"

#ifndef OMITBAD
void CWE665_Improper_Initialization__char_cat_10_bad(void);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE665_Improper_Initialization__char_cat_10_good(void);
#endif /* OMITGOOD */

void run_bad(void) {
    printf("Running BAD test...\n");
    CWE665_Improper_Initialization__char_cat_10_bad();
    printf("Finished BAD test.\n");
}

void run_good(void) {
    printf("Running GOOD test...\n");
    CWE665_Improper_Initialization__char_cat_10_good();
    printf("Finished GOOD test.\n");
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