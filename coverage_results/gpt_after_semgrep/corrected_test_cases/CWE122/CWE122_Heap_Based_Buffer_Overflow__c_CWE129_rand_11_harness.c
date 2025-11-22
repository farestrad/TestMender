// ```c
#include <stdio.h>
#include <stdlib.h>
#include "std_testcase.h"

#ifndef OMITBAD
void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_rand_11_bad(void);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_rand_11_good(void);
#endif /* OMITGOOD */

void run_bad(void) {
    printf("Running bad case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE129_rand_11_bad();
}

void run_good(void) {
    printf("Running good case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE129_rand_11_good();
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