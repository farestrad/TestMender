// ```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Function prototypes for the Juliet entry points */
void CWE190_Integer_Overflow__unsigned_int_rand_add_65_bad(void);
void CWE190_Integer_Overflow__unsigned_int_rand_add_65_good(void);

/* Run function for the bad case */
void run_bad(void) {
    printf("Running bad test...\n");
    CWE190_Integer_Overflow__unsigned_int_rand_add_65_bad();
}

/* Run function for the good case */
void run_good(void) {
    printf("Running good test...\n");
    CWE190_Integer_Overflow__unsigned_int_rand_add_65_good();
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