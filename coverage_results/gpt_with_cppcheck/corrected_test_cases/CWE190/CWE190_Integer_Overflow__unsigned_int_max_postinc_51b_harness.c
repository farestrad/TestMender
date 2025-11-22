#include "std_testcase.h"

#ifndef OMITBAD
void CWE190_Integer_Overflow__unsigned_int_max_postinc_51b_badSink(unsigned int data);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE190_Integer_Overflow__unsigned_int_max_postinc_51b_goodG2BSink(unsigned int data);
void CWE190_Integer_Overflow__unsigned_int_max_postinc_51b_goodB2GSink(unsigned int data);
#endif /* OMITGOOD */

void run_bad(void) {
    unsigned int data;
    // Set data to max value to induce overflow
    data = UINT_MAX;

    printf("Running bad test case...\n");
    CWE190_Integer_Overflow__unsigned_int_max_postinc_51b_badSink(data);
}

void run_good(void) {
    unsigned int data;
    
    // Good source, set data to a small, non-zero number
    data = 2;
    
    printf("Running good test case (Good Source)...\n");
    CWE190_Integer_Overflow__unsigned_int_max_postinc_51b_goodG2BSink(data);
    
    // Testing the good sink with a max value to illustrate prevention
    data = UINT_MAX;
    printf("Running good test case (Good Sink)...\n");
    CWE190_Integer_Overflow__unsigned_int_max_postinc_51b_goodB2GSink(data);
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}