```c
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "std_testcase.h"

#ifndef OMITBAD
void CWE190_Integer_Overflow__int_rand_preinc_15_bad(void);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE190_Integer_Overflow__int_rand_preinc_15_good(void);
#endif /* OMITGOOD */

void run_bad(void) {
    printf("Running BAD test case...\n");
    CWE190_Integer_Overflow__int_rand_preinc_15_bad();
    printf("Finished BAD test case.\n");
}

void run_good(void) {
    printf("Running GOOD test case...\n");
    CWE190_Integer_Overflow__int_rand_preinc_15_good();
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
```