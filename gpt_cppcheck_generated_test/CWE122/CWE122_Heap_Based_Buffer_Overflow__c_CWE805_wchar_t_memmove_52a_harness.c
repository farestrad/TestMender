```c
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

// Prototypes for the Juliet entry functions
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52_good(void);

// Function to run the bad test case
void run_bad(void) {
    printf("Running bad case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52_bad();
    printf("Finished bad case.\n");
}

// Function to run the good test case
void run_good(void) {
    printf("Running good case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52_good();
    printf("Finished good case.\n");
}

// Main function
int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```