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

static const char SRC_STRING[11] = "AAAAAAAAAA"; // 10 chars + null terminator

void run_bad(void) {
    char *data;
    // POTENTIAL FLAW: Allocate memory for only 10 chars, no space for NULL terminator
    data = (char*)malloc(10 * sizeof(char));
    if (data == NULL) { exit(-1); }
    // Fill buffer with 10 chars (no null terminator; simulates the "bad" configuration)
    memcpy(data, SRC_STRING, 10 * sizeof(char));
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_loop_65b_badSink(data);
    // The sink is responsible for freeing if appropriate according to Juliet pattern
}

void run_good(void) {
    char *data;
    // FIX: Allocate enough memory for 10 chars + NULL terminator
    data = (char*)malloc(11 * sizeof(char));
    if (data == NULL) { exit(-1); }
    // Properly copy the source including null terminator
    memcpy(data, SRC_STRING, 11 * sizeof(char));
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_loop_65b_goodG2BSink(data);
    // The sink is responsible for freeing if appropriate according to Juliet pattern
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