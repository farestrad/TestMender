```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Declare prototypes for the Juliet entry functions */
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_cpy_17_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_cpy_17_good(void);

void run_bad(void) {
    printf("Running bad case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_cpy_17_bad();
}

void run_good(void) {
    printf("Running good case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_cpy_17_good();
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