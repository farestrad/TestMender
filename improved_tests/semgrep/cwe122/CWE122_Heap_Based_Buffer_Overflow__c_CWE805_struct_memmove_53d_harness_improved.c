```c
#include <stdio.h>
#include <stdlib.h>
#include "std_testcase.h"

void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memmove_53d_badSink(twoIntsStruct * data);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memmove_53d_goodG2BSink(twoIntsStruct * data);

void run_bad(void) {
    twoIntsStruct * data;
    /* Allocate a buffer that is too small and will trigger an overflow in the BAD path */
    data = (twoIntsStruct *)malloc(50 * sizeof(twoIntsStruct)); // BAD: insufficient buffer
    if (data == NULL) { exit(-1); }
    /* Fill with some values (optional but harmless) */
    for (size_t i = 0; i < 50; ++i) {
        data[i].intOne = (int)i;
        data[i].intTwo = (int)i + 100;
    }
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memmove_53d_badSink(data);
    free(data);
}

void run_good(void) {
    twoIntsStruct * data;
    /* Allocate a buffer large enough for GOOD path to avoid overflow */
    data = (twoIntsStruct *)malloc(100 * sizeof(twoIntsStruct)); // GOOD: sufficient buffer
    if (data == NULL) { exit(-1); }
    for (size_t i = 0; i < 100; ++i) {
        data[i].intOne = (int)i;
        data[i].intTwo = (int)i + 100;
    }
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memmove_53d_goodG2BSink(data);
    free(data);
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