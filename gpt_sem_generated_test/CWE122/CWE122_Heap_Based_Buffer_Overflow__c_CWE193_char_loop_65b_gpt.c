#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SRC_STRING "AAAAAAAAAA"

void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_loop_65b_badSink(char * data) {
    char source[10+1] = SRC_STRING;
    size_t i, sourceLen;
    sourceLen = strlen(source);
    for (i = 0; i < sourceLen + 1; i++) {
        data[i] = source[i];
    }
    printf("Result: %s\n", data);
    free(data);
}

void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_loop_65b_goodG2BSink(char * data) {
    char source[10+1] = SRC_STRING;
    size_t i, sourceLen;
    sourceLen = strlen(source);
    for (i = 0; i < sourceLen + 1; i++) {
        data[i] = source[i];
    }
    printf("Result: %s\n", data);
    free(data);
}

void testBad() {
    char *data = (char *)malloc(10 * sizeof(char)); // Allocate insufficient space
    if (data == NULL) {
        exit(1);
    }
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_loop_65b_badSink(data);
}

void testGood() {
    char *data = (char *)malloc(11 * sizeof(char)); // Allocate sufficient space for NULL terminator
    if (data == NULL) {
        exit(1);
    }
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_loop_65b_goodG2BSink(data);
}

int main() {
    printf("Running bad test...\n");
    testBad();
    
    printf("Running good test...\n");
    testGood();
    
    return 0;
}