// ```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_char_snprintf_64b_badSink(void * dataVoidPtr);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_char_snprintf_64b_goodG2BSink(void * dataVoidPtr);

void run_bad(void) {
    char * data;
    /* FLAW: Allocate a small buffer */
    data = (char *)malloc(50 * sizeof(char));
    if (data == NULL) { exit(-1); }
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_char_snprintf_64b_badSink(&data);
}

void run_good(void) {
    char * data;
    /* GOOD: Allocate a large buffer */
    data = (char *)malloc(100 * sizeof(char));
    if (data == NULL) { exit(-1); }
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_char_snprintf_64b_goodG2BSink(&data);
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