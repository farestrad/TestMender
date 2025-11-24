```c
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

// Prototypes for the Juliet entry functions
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52_good(void);

/*
 * The GOOD path is required to run cleanly with no AddressSanitizer
 * errors and no crashes. No extra setup is required except for calling
 * the Juliet good() function; do NOT tamper with memory in the harness.
 * The BAD path should reliably hit an out-of-bounds write.
 */

int main(void) {
#ifdef TEST_MODE_BAD
    printf("Running BAD case:\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52_bad();
    printf("Completed BAD case.\n");
#else
    printf("Running GOOD case:\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52_good();
    printf("Completed GOOD case.\n");
#endif
    return 0;
}
```