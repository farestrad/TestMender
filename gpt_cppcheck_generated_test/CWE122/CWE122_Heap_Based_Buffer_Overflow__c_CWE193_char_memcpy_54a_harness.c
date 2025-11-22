```c
#include <stdio.h>
#include <stdlib.h>

// Prototypes for the Juliet entry functions
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_memcpy_54_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_memcpy_54_good(void);

void run_bad(void) {
    printf("Running BAD test...\n");
    // Call the bad function to demonstrate the vulnerability
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_memcpy_54_bad();
    printf("Finished BAD test.\n");
}

void run_good(void) {
    printf("Running GOOD test...\n");
    // Call the good function to demonstrate safe behavior
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_memcpy_54_good();
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