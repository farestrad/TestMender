// ```c
#include <stdio.h>
#include <stdlib.h>

void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_large_66_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_large_66_good(void);

void run_bad(void) {
    printf("Running bad test case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE129_large_66_bad();
    printf("Finished bad test case.\n");
}

void run_good(void) {
    printf("Running good test case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE129_large_66_good();
    printf("Finished good test case.\n");
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
// ```