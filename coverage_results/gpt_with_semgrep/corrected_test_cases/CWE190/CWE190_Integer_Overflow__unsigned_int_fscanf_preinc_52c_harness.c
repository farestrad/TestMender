// ```c
#include <stdio.h>
#include <limits.h>

#ifndef OMITBAD
void CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_52c_badSink(unsigned int data);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_52c_goodG2BSink(unsigned int data);
void CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_52c_goodB2GSink(unsigned int data);
#endif /* OMITGOOD */

void run_bad(void) {
    unsigned int data;
    // Simulating bad input that leads to potential overflow
    data = UINT_MAX; // Set to max unsigned int to trigger overflow
    printf("Running bad case with data: %u\n", data);
    CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_52c_badSink(data);
}

void run_good(void) {
    unsigned int data;
    // Simulating good input that avoids overflow
    data = 2; // Set to a small, non-zero number
    printf("Running good case with data: %u\n", data);
    CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_52c_goodB2GSink(data);
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