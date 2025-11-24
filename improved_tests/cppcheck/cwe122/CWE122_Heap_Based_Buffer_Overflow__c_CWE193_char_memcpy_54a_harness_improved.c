```c
#include <stdio.h>
#include <stdlib.h>

// Prototypes for the Juliet entry functions
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_memcpy_54_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_memcpy_54_good(void);

int main(void) {
#ifdef TEST_MODE_BAD
    printf("Running BAD test...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_memcpy_54_bad();
    printf("Finished BAD test.\n");
#else
    printf("Running GOOD test...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_memcpy_54_good();
    printf("Finished GOOD test.\n");
#endif
    return 0;
}
```