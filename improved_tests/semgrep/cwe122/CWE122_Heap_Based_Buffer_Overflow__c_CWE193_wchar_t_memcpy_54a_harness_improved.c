```c
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

// Prototypes for the Juliet entry functions
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_wchar_t_memcpy_54_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_wchar_t_memcpy_54_good(void);

// Main function to control test execution
int main(void) {
#ifdef TEST_MODE_BAD
    // BAD path: should trigger heap buffer overflow
    printf("Running BAD test case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_wchar_t_memcpy_54_bad();
    printf("Finished BAD test case.\n");
#else
    // GOOD path: ensure good behavior, should not trigger AddressSanitizer
    printf("Running GOOD test case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_wchar_t_memcpy_54_good();
    printf("Finished GOOD test case.\n");
#endif
    return 0;
}
```