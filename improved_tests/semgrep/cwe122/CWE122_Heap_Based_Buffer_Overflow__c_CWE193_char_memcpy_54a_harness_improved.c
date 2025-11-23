```c
#include <stdio.h>
#include <stdlib.h>

/* Prototype the Juliet test case entry points */
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_memcpy_54_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_memcpy_54_good(void);

/* 
 * The Juliet test harness does not require any input preparation for this variant,
 * but we ensure that the program exits with the right code so that
 * AddressSanitizer can report the intended memory error for the bad case,
 * and that the good case completes successfully. 
 */

/* We avoid catching signals or errors so that ASan can handle the crash for BAD. */

int main(void) {
#ifdef TEST_MODE_BAD
    printf("Running bad case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_memcpy_54_bad();
    printf("Finished bad case. (unexpected)\n");
#else
    printf("Running good case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_memcpy_54_good();
    printf("Finished good case.\n");
#endif
    return 0;
}
```