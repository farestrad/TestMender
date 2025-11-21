// ```c
#include "std_testcase.h"

#define CHAR_ARRAY_SIZE (3 * sizeof(data) + 2)

void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_fgets_11_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_fgets_11_good(void);

void run_bad(void) {
    printf("Running bad test...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE129_fgets_11_bad();
    printf("Finished bad test.\n");
}

void run_good(void) {
    printf("Running good test...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE129_fgets_11_good();
    printf("Finished good test.\n");
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