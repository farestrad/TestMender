```c
#include "std_testcase.h"

#ifndef OMITBAD
void CWE190_Integer_Overflow__unsigned_int_rand_add_10_bad(void);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE190_Integer_Overflow__unsigned_int_rand_add_10_good(void);
#endif /* OMITGOOD */

void run_bad(void) {
    printf("Running BAD test...\n");
    CWE190_Integer_Overflow__unsigned_int_rand_add_10_bad();
}

void run_good(void) {
    printf("Running GOOD test...\n");
    CWE190_Integer_Overflow__unsigned_int_rand_add_10_good();
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