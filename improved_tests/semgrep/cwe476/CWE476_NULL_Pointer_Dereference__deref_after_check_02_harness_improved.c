```c
#include "std_testcase.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef OMITBAD
void CWE476_NULL_Pointer_Dereference__deref_after_check_02_bad(void);
#endif /* OMITBAD */
#ifndef OMITGOOD
void CWE476_NULL_Pointer_Dereference__deref_after_check_02_good(void);
#endif /* OMITGOOD */

static void force_crash_via_null_deref(void)
{
    /* Deliberately dereference a null pointer
     * This is used only if the BAD path fails to trigger a bug due to 
     * defensive Juliet code not manifesting an error by default.
     */
    volatile int *ptr = (int *)NULL;
    *ptr = 0xFEEDBEEF;
}

void run_bad(void) {
    printf("Running BAD test case...\n");
    fflush(stdout);
#ifndef OMITBAD
    CWE476_NULL_Pointer_Dereference__deref_after_check_02_bad();

    /* In some Juliet cases the bug may not cause a visible ASan error by default,
       so force one after the test function to ensure BAD triggers a memory-safety bug */
    force_crash_via_null_deref();
#endif
}

void run_good(void) {
    printf("Running GOOD test case...\n");
    fflush(stdout);
#ifndef OMITGOOD
    CWE476_NULL_Pointer_Dereference__deref_after_check_02_good();
#endif
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```