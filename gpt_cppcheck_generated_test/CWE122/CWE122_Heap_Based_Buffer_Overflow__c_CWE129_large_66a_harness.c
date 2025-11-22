```c
#include <stdio.h>

// Prototypes for the Juliet entry functions
void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_large_66_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_large_66_good(void);

void run_bad(void) {
    printf("Running BAD test...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE129_large_66_bad();
    printf("Finished BAD test.\n");
}

void run_good(void) {
    printf("Running GOOD test...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE129_large_66_good();
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