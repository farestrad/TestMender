#include "std_testcase.h"

#ifndef OMITBAD
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_cpy_17_bad(void);
#endif

#ifndef OMITGOOD
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_cpy_17_good(void);
#endif

void run_bad(void) {
#ifndef OMITBAD
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_cpy_17_bad();
#endif
}

void run_good(void) {
#ifndef OMITGOOD
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_cpy_17_good();
#endif
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
