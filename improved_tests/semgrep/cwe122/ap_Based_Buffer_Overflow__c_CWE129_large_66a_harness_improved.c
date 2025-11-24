```c
#include "std_testcase.h"

/* The Juliet function prototypes for bad and good paths */
void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_large_66_bad();
void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_large_66_good();

/* This main invokes a clean GOOD execution, then a BAD that triggers AddressSanitizer */
int main(int argc, char * argv[])
{
    /* GOOD: This should run without errors */
    printLine("Calling good()...");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE129_large_66_good();
    printLine("Finished good()");

    /* BAD: This should cause AddressSanitizer to report a heap buffer overflow */
    printLine("Calling bad()...");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE129_large_66_bad();
    printLine("Finished bad()");

    return 0;
}
```