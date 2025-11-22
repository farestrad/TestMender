// ```c
#include <stdio.h>
#include <stdlib.h>
#include "std_testcase.h"

void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memmove_53d_badSink(twoIntsStruct * data);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memmove_53d_goodG2BSink(twoIntsStruct * data);

void run_bad(void) {
    twoIntsStruct * data;
    // Allocate a small buffer
    data = (twoIntsStruct *)malloc(50 * sizeof(twoIntsStruct)); // POTENTIAL FLAW
    if (data == NULL) { exit(-1); }
    // Call the bad sink
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memmove_53d_badSink(data);
}

void run_good(void) {
    twoIntsStruct * data;
    // Allocate a large buffer
    data = (twoIntsStruct *)malloc(100 * sizeof(twoIntsStruct)); // Good allocation
    if (data == NULL) { exit(-1); }
    // Call the good sink
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memmove_53d_goodG2BSink(data);
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