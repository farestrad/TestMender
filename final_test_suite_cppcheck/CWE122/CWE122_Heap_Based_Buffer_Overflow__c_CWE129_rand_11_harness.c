```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_rand_11_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_rand_11_good(void);

void run_bad(void) {
    printf("Running BAD test case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE129_rand_11_bad();
}

void run_good(void) {
    printf("Running GOOD test case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE129_rand_11_good();
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