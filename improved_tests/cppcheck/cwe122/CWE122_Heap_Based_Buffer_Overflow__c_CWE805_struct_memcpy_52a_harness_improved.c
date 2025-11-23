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

int main(void) {
    /* No heap or stack data to initialize here - Juliet sources
       manage allocation and invocation details. */
#ifdef TEST_MODE_BAD
    printf("Running BAD test...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52_bad();
    printf("Finished BAD test.\n");
#else
    printf("Running GOOD test...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52_good();
    printf("Finished GOOD test.\n");
#endif
    return 0;
}
```