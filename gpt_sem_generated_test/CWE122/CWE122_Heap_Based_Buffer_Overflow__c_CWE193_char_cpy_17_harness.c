```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_cpy_17_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_cpy_17_good(void);

void run_bad(void) {
    printf("Running BAD case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_cpy_17_bad();
    printf("Finished BAD case.\n");
}

void run_good(void) {
    printf("Running GOOD case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_cpy_17_good();
    printf("Finished GOOD case.\n");
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