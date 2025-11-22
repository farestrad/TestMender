// ```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Function prototypes for the Juliet entry functions */
void CWE190_Integer_Overflow__unsigned_int_rand_add_65_bad(void);
void CWE190_Integer_Overflow__unsigned_int_rand_add_65_good(void);

/* Function to run the BAD case */
void run_bad(void) {
    printf("Running BAD case...\n");
    CWE190_Integer_Overflow__unsigned_int_rand_add_65_bad();
}

/* Function to run the GOOD case */
void run_good(void) {
    printf("Running GOOD case...\n");
    CWE190_Integer_Overflow__unsigned_int_rand_add_65_good();
}

/* Main function to drive the harness */
int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
// ```