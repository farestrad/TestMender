#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int intOne;
    int intTwo;
} twoIntsStruct;

void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memmove_53d_badSink(twoIntsStruct * data);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memmove_53d_goodG2BSink(twoIntsStruct * data);

void testBadCase() {
    // Allocate a small buffer, which is insufficient for 100 twoIntsStruct
    twoIntsStruct *data = (twoIntsStruct *)malloc(5 * sizeof(twoIntsStruct));
    if (data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    
    // Call the bad sink, expecting a buffer overflow
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memmove_53d_badSink(data);
}

void testGoodCase() {
    // Allocate a large buffer sufficient for 100 twoIntsStruct
    twoIntsStruct *data = (twoIntsStruct *)malloc(100 * sizeof(twoIntsStruct));
    if (data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    
    // Call the good sink
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memmove_53d_goodG2BSink(data);
}

int main() {
    printf("Testing bad case...\n");
    testBadCase();

    printf("Testing good case...\n");
    testGoodCase();

    return 0;
}