// ```c
#include "std_testcase.h"

#ifndef OMITBAD
void CWE190_Integer_Overflow__unsigned_int_fscanf_multiply_65b_badSink(unsigned int data);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE190_Integer_Overflow__unsigned_int_fscanf_multiply_65b_goodG2BSink(unsigned int data);
void CWE190_Integer_Overflow__unsigned_int_fscanf_multiply_65b_goodB2GSink(unsigned int data);
#endif /* OMITGOOD */

void run_bad(void) {
    unsigned int data;
    /* FLAW: Read data from the console using fscanf() */
    fscanf(stdin, "%u", &data);
    CWE190_Integer_Overflow__unsigned_int_fscanf_multiply_65b_badSink(data);
}

void run_good(void) {
    unsigned int data;
    /* FIX: Set data to a small, non-zero number */
    data = 2;
    CWE190_Integer_Overflow__unsigned_int_fscanf_multiply_65b_goodG2BSink(data);
    
    /* Additional good case to demonstrate overflow prevention */
    data = UINT_MAX; // may trigger overflow if multiplied
    CWE190_Integer_Overflow__unsigned_int_fscanf_multiply_65b_goodB2GSink(data);
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