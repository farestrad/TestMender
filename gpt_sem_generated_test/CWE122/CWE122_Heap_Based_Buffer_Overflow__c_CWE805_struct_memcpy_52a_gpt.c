#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int x;
    int y;
} twoIntsStruct;

void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52b_badSink(twoIntsStruct * data);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52b_goodG2BSink(twoIntsStruct * data);

void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52_bad() {
    twoIntsStruct * data;
    data = NULL;
    data = (twoIntsStruct *)malloc(50 * sizeof(twoIntsStruct));
    if (data == NULL) { exit(-1); }
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52b_badSink(data);
}

void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52_good() {
    twoIntsStruct * data;
    data = NULL;
    data = (twoIntsStruct *)malloc(100 * sizeof(twoIntsStruct));
    if (data == NULL) { exit(-1); }
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52b_goodG2BSink(data);
}

void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52b_badSink(twoIntsStruct * data) {
    twoIntsStruct src[100];
    memset(src, 0, sizeof(src));
    memcpy(data, src, 100 * sizeof(twoIntsStruct));
    printf("Executed bad sink\n");
}

void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52b_goodG2BSink(twoIntsStruct * data) {
    twoIntsStruct src[100];
    memset(src, 0, sizeof(src));
    memcpy(data, src, 100 * sizeof(twoIntsStruct));
    printf("Executed good G2B sink\n");
}

int main(int argc, char * argv[]) {
    printf("Calling bad()...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52_bad();
    printf("Finished bad()\n");

    printf("Calling good()...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_struct_memcpy_52_good();
    printf("Finished good()\n");

    return 0;
}