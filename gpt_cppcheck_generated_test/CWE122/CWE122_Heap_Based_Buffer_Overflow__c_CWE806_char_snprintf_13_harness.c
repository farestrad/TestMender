```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CWE122_Heap_Based_Buffer_Overflow__c_CWE806_char_snprintf_13_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE806_char_snprintf_13_good(void);

void run_bad(void) {
    printf("Running BAD test...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE806_char_snprintf_13_bad();
}

void run_good(void) {
    printf("Running GOOD test...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE806_char_snprintf_13_good();
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