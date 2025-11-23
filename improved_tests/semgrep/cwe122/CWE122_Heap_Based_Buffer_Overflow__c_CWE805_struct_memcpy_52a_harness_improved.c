```c
#include <stdio.h>
#include <stdlib.h>
#include "std_testcase.h"

typedef struct _twoIntsStruct {
    int intOne;
    int intTwo;
} twoIntsStruct;

void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52_badSink(twoIntsStruct * data);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52_goodG2BSink(twoIntsStruct * data);

/* Manually emulate BAD path allocation as in the original 52a/bad */
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52_bad(void)
{
    twoIntsStruct * data;
    data = NULL;
    /* FLAW: Allocate using malloc() but only enough for 50 twoIntsStructs */
    data = (twoIntsStruct *)malloc(50 * sizeof(twoIntsStruct));
    if (data == NULL) {exit(-1);}
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52_badSink(data);
    free(data);
}

/* Manually emulate GOOD path allocation as in the original 52a/good */
static void goodG2B(void)
{
    twoIntsStruct * data;
    data = NULL;
    /* FIX: Allocate and point data to a large buffer that is at least as large as the large buffer used in the copy in the sink */
    data = (twoIntsStruct *)malloc(100 * sizeof(twoIntsStruct));
    if (data == NULL) {exit(-1);}
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52_goodG2BSink(data);
    free(data);
}

void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52_good(void)
{
    goodG2B();
}

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
```