// ```c
#include <stdio.h>
#include "std_testcase.h"

/* Function prototypes */
void CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_61b_bad(void);
void CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_61b_goodG2B(void);
void CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_61b_goodB2G(void);

void run_bad(void) {
    printf("Running bad test...\n");
    CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_61b_bad();
}

void run_good(void) {
    printf("Running good test...\n");
    CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_61b_goodG2B();
    CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_61b_goodB2G();
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