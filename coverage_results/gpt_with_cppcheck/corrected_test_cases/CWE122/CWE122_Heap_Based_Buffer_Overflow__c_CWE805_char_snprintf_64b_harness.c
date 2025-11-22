// ```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_char_snprintf_64b_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_char_snprintf_64b_good(void);

void run_bad(void) {
    char * data = (char *)malloc(50 * sizeof(char)); // Allocate small buffer
    if (data == NULL) { exit(1); }
    void * dataVoidPtr = &data;
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_char_snprintf_64b_badSink(dataVoidPtr);
    printf("Executed BAD function.\n");
}

void run_good(void) {
    char * data = (char *)malloc(100 * sizeof(char)); // Allocate large buffer
    if (data == NULL) { exit(1); }
    void * dataVoidPtr = &data;
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_char_snprintf_64b_goodG2BSink(dataVoidPtr);
    printf("Executed GOOD function.\n");
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