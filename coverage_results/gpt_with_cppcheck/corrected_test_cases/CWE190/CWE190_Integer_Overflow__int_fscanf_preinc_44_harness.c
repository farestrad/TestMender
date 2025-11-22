// ```c
#include <stdio.h>
#include <limits.h>

// Prototypes for the Juliet tests
#ifndef OMITBAD
void CWE190_Integer_Overflow__int_fscanf_preinc_44_bad(void);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE190_Integer_Overflow__int_fscanf_preinc_44_good(void);
#endif /* OMITGOOD */

// Function to run the bad test case
void run_bad(void) {
    printf("Running BAD test case...\n");
    CWE190_Integer_Overflow__int_fscanf_preinc_44_bad();
}

// Function to run the good test cases
void run_good(void) {
    printf("Running GOOD test cases...\n");
    CWE190_Integer_Overflow__int_fscanf_preinc_44_good();
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