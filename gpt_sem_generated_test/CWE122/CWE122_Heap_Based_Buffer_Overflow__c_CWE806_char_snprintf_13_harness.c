```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prototypes for the functions in the original Juliet source code
void CWE122_Heap_Based_Buffer_Overflow__c_CWE806_char_snprintf_13_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE806_char_snprintf_13_good(void);

void run_bad(void) {
    printf("Running bad case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE806_char_snprintf_13_bad();
    printf("Finished running bad case.\n");
}

void run_good(void) {
    printf("Running good case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE806_char_snprintf_13_good();
    printf("Finished running good case.\n");
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