// ```c
#include "std_testcase.h"

#ifndef OMITBAD
void CWE476_NULL_Pointer_Dereference__int64_t_17_bad(void);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE476_NULL_Pointer_Dereference__int64_t_17_good(void);
#endif /* OMITGOOD */

void run_bad(void) {
    printf("Running BAD test...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_17_bad();
}

void run_good(void) {
    printf("Running GOOD test...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_17_good();
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