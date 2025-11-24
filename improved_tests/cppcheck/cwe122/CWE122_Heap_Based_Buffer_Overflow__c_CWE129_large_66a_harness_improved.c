#include "std_testcase.h"

#ifndef OMITBAD
void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_large_66_bad(void);
#endif

#ifndef OMITGOOD
void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_large_66_good(void);
#endif

void run_bad(void) {
#ifndef OMITBAD
    CWE122_Heap_Based_Buffer_Overflow__c_CWE129_large_66_bad();
#endif
}

void run_good(void) {
#ifndef OMITGOOD
    CWE122_Heap_Based_Buffer_Overflow__c_CWE129_large_66_good();
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
