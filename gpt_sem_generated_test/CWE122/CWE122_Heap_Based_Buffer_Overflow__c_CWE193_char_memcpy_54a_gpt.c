#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SRC_STRING "AAAAAAAAAA"
#define BUFFER_SIZE 10 // Size for the bad case without the null terminator
#define BUFFER_SIZE_SAFE 11 // Size for the good case with the null terminator

void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_memcpy_54b_badSink(char * data);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_memcpy_54b_goodG2BSink(char * data);

void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_memcpy_54_bad() {
    char * data;
    data = NULL;
    data = (char *)malloc(BUFFER_SIZE * sizeof(char)); // FLAW
    if (data == NULL) { exit(-1); }
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_memcpy_54b_badSink(data);
    free(data);
}

void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_memcpy_54_good() {
    char * data;
    data = NULL;
    data = (char *)malloc(BUFFER_SIZE_SAFE * sizeof(char)); // FIX
    if (data == NULL) { exit(-1); }
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_memcpy_54b_goodG2BSink(data);
    free(data);
}

void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_memcpy_54b_badSink(char * data) {
    memcpy(data, SRC_STRING, strlen(SRC_STRING)); // Possible overflow
    data[BUFFER_SIZE - 1] = '\0'; // Attempt to null terminate but may crash
    printf("Bad case output: %s\n", data);
}

void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_memcpy_54b_goodG2BSink(char * data) {
    memcpy(data, SRC_STRING, strlen(SRC_STRING) + 1); // Safe copy
    printf("Good case output: %s\n", data);
}

int main(int argc, char * argv[]) {
    printf("Testing bad function:\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_memcpy_54_bad();
    
    printf("Testing good function:\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_memcpy_54_good();
    
    return 0;
}