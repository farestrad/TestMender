```c
#include "std_testcase.h"
#include <wchar.h>

#ifndef OMITBAD
void CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_bad(void);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_good(void);
#endif /* OMITGOOD */

void run_bad(void) {
    printLine("Running bad case...");
    CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_bad();
    printLine("Completed bad case.");
}

void run_good(void) {
    printLine("Running good case...");
    CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_good();
    printLine("Completed good case.");
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