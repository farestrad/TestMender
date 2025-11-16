```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declare prototypes for the Juliet entry functions
#ifndef OMITBAD
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_loop_65b_badSink(char * data);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_loop_65b_goodG2BSink(char * data);
#endif /* OMITGOOD */

void run_bad(void) {
    char *data;
    // POTENTIAL FLAW: Allocate memory without space for NULL terminator
    data = (char*)malloc(10 * sizeof(char)); 
    if (data == NULL) { exit(-1); }
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_loop_65b_badSink(data);
}

void run_good(void) {
    char *data;
    // Allocate enough memory for the string and the NULL terminator
    data = (char*)malloc(11 * sizeof(char)); 
    if (data == NULL) { exit(-1); }
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_loop_65b_goodG2BSink(data);
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