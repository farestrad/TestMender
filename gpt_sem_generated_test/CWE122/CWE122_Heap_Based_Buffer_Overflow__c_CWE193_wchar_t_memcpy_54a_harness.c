```c
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

// Prototypes for the Juliet entry functions
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_wchar_t_memcpy_54_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_wchar_t_memcpy_54_good(void);

// Function to run the BAD test case
void run_bad(void) {
    printf("Running BAD test case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_wchar_t_memcpy_54_bad();
    printf("Finished BAD test case.\n");
}

// Function to run the GOOD test case
void run_good(void) {
    printf("Running GOOD test case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_wchar_t_memcpy_54_good();
    printf("Finished GOOD test case.\n");
}

// Main function to control test execution
int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```