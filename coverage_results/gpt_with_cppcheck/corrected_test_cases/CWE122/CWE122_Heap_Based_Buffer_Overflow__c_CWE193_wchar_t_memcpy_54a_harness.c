// ```c
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

/* Prototype declarations for the Juliet entry points */
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_wchar_t_memcpy_54_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_wchar_t_memcpy_54_good(void);

/* Function to run the bad testcase */
void run_bad(void) {
    printf("Running bad test case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_wchar_t_memcpy_54_bad();
    printf("Finished bad test case.\n");
}

/* Function to run the good testcase */
void run_good(void) {
    printf("Running good test case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_wchar_t_memcpy_54_good();
    printf("Finished good test case.\n");
}

/* Main function */
int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
// ```