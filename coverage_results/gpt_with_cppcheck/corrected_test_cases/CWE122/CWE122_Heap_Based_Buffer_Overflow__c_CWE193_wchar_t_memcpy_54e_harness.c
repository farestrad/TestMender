// ```c
#include "std_testcase.h"

#ifndef _WIN32
#include <wchar.h>
#endif

// Prototypes for the Juliet entry functions
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_wchar_t_memcpy_54e_badSink(wchar_t * data);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_wchar_t_memcpy_54e_goodG2BSink(wchar_t * data);

// Bad function to drive the vulnerability
void run_bad(void) {
    wchar_t * data = (wchar_t *)malloc(10 * sizeof(wchar_t)); // Allocate insufficient space
    if (data == NULL) { exit(EXIT_FAILURE); }
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_wchar_t_memcpy_54e_badSink(data);
}

// Good function to drive the safe path
void run_good(void) {
    wchar_t * data = (wchar_t *)malloc((10 + 1) * sizeof(wchar_t)); // Allocate sufficient space
    if (data == NULL) { exit(EXIT_FAILURE); }
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_wchar_t_memcpy_54e_goodG2BSink(data);
}

// Main function
int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
// ```