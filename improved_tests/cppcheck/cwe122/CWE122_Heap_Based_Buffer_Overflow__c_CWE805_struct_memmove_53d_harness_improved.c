```c
#include <stdio.h>
#include <stdlib.h>
#include "std_testcase.h"

/* Prototypes for the Juliet entry functions */
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memmove_53d_badSink(twoIntsStruct * data);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memmove_53d_goodG2BSink(twoIntsStruct * data);

/*
 * The vulnerable code expects to copy 100 elements into the 'data' buffer via memmove.
 * To ensure that GOOD passes and BAD triggers an overflow, allocate 100 for GOOD and 50 for BAD.
 */

/* Function to run the BAD test case */
void run_bad(void) {
    twoIntsStruct * data;
    /* Allocate a buffer that is too small */
    data = (twoIntsStruct *)malloc(50 * sizeof(twoIntsStruct));
    if (data == NULL) {
        exit(1);
    }
    printf("Running BAD test case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memmove_53d_badSink(data);
    free(data);
}

/* Function to run the GOOD test case */
void run_good(void) {
    twoIntsStruct * data;
    /* Allocate a buffer that is large enough */
    data = (twoIntsStruct *)malloc(100 * sizeof(twoIntsStruct));
    if (data == NULL) {
        exit(1);
    }
    printf("Running GOOD test case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memmove_53d_goodG2BSink(data);
    free(data);
}

/* Main function */
int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```