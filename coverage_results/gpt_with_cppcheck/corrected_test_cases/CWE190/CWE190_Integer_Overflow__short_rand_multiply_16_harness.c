// ```c
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#ifndef OMITBAD
void CWE190_Integer_Overflow__short_rand_multiply_16_bad(void);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE190_Integer_Overflow__short_rand_multiply_16_good(void);
#endif /* OMITGOOD */

void run_bad(void) {
    printf("Running bad test...\n");
    CWE190_Integer_Overflow__short_rand_multiply_16_bad();
}

void run_good(void) {
    printf("Running good test...\n");
    CWE190_Integer_Overflow__short_rand_multiply_16_good();
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