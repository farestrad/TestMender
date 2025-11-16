```c
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

// Prototypes for the Juliet entry functions
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52_good(void);

// Function to run the BAD case
void run_bad(void) {
    printf("Running BAD case:\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52_bad();
    printf("Completed BAD case.\n");
}

// Function to run the GOOD case
void run_good(void) {
    printf("Running GOOD case:\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52_good();
    printf("Completed GOOD case.\n");
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