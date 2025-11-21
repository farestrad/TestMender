// ```c
#include "std_testcase.h"

// Prototypes for the Juliet entry functions
void CWE190_Integer_Overflow__short_rand_multiply_16_bad(void);
void CWE190_Integer_Overflow__short_rand_multiply_16_good(void);

// Function to run the BAD case
void run_bad(void) {
    printf("Running BAD test case...\n");
    CWE190_Integer_Overflow__short_rand_multiply_16_bad();
}

// Function to run the GOOD case
void run_good(void) {
    printf("Running GOOD test case...\n");
    CWE190_Integer_Overflow__short_rand_multiply_16_good();
}

// Main function to switch between BAD and GOOD
int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
// ```