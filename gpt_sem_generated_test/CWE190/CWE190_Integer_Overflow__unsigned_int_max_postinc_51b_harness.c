```c
#include <stdio.h>
#include <limits.h>

// Function prototypes for the Juliet test cases
#ifndef OMITBAD
void CWE190_Integer_Overflow__unsigned_int_max_postinc_51b_badSink(unsigned int data);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE190_Integer_Overflow__unsigned_int_max_postinc_51b_goodG2BSink(unsigned int data);
void CWE190_Integer_Overflow__unsigned_int_max_postinc_51b_goodB2GSink(unsigned int data);
#endif /* OMITGOOD */

void run_bad(void) {
    // Set data to the maximum value for unsigned int to trigger overflow
    unsigned int data = UINT_MAX;
    printf("Running bad case with data = %u\n", data);
    CWE190_Integer_Overflow__unsigned_int_max_postinc_51b_badSink(data);
}

void run_good(void) {
    // Good source: Set data to a small non-zero number
    unsigned int data = 2;
    printf("Running good case with data = %u\n", data);
    CWE190_Integer_Overflow__unsigned_int_max_postinc_51b_goodG2BSink(data);

    // Also test the good sink with potentially large data
    data = UINT_MAX;
    printf("Testing good case for boundary with data = %u\n", data);
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
```