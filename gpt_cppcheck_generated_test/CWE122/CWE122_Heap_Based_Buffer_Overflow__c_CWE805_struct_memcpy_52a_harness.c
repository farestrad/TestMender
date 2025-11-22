```c
#include <stdio.h>
#include <stdlib.h>
#include "std_testcase.h"

#ifndef OMITBAD
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52_bad(void);
#endif /* OMITBAD */
#ifndef OMITGOOD
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52_good(void);
#endif /* OMITGOOD */

void run_bad(void) {
    printf("Running BAD test...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52_bad();
    printf("Finished BAD test.\n");
}

void run_good(void) {
    printf("Running GOOD test...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52_good();
    printf("Finished GOOD test.\n");
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