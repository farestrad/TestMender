// ```c
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

// Declare prototypes for the Juliet entry functions
void CWE190_Integer_Overflow__int_fgets_square_41_bad(void);
void CWE190_Integer_Overflow__int_fgets_square_41_good(void);

void run_bad(void) {
    printf("Running BAD test case...\n");
    // Call the BAD entry function
    CWE190_Integer_Overflow__int_fgets_square_41_bad();
}

void run_good(void) {
    printf("Running GOOD test cases...\n");
    // Call the GOOD entry function
    CWE190_Integer_Overflow__int_fgets_square_41_good();
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