// ```c
#include <stdio.h>
#include <stdlib.h>
#include "std_testcase.h"

typedef struct _twoIntsStruct {
    int intOne;
    int intTwo;
} twoIntsStruct;

void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52_good(void);

void run_bad(void) {
    printf("Running bad()...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52_bad();
    printf("Finished bad()\n");
}

void run_good(void) {
    printf("Running good()...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52_good();
    printf("Finished good()\n");
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