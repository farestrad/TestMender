```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_char_snprintf_64b_badSink(void * dataVoidPtr);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_char_snprintf_64b_goodG2BSink(void * dataVoidPtr);

void run_bad(void) {
    /* Allocate a small buffer, intentionally too small */
    char * data = (char *)malloc(50 * sizeof(char));
    if (data == NULL) { exit(1); }
    /* Fill with something so the buffer is initialized */
    memset(data, 'A', 49);
    data[49] = '\0';
    void * dataVoidPtr = &data;
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_char_snprintf_64b_badSink(dataVoidPtr);
    /* Cleanup (though we're expecting a crash for BAD) */
    free(data);
}

void run_good(void) {
    /* Allocate a large enough buffer */
    char * data = (char *)malloc(100 * sizeof(char));
    if (data == NULL) { exit(1); }
    /* Fill with something so the buffer is initialized */
    memset(data, 'A', 99);
    data[99] = '\0';
    void * dataVoidPtr = &data;
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_char_snprintf_64b_goodG2BSink(dataVoidPtr);
    printf("Executed GOOD function.\n");
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