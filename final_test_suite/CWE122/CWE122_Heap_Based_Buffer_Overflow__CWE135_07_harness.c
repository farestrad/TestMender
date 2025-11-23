```c
#include "std_testcase.h"
#include <wchar.h>

#ifndef OMITBAD
void CWE122_Heap_Based_Buffer_Overflow__CWE135_07_bad(void);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE122_Heap_Based_Buffer_Overflow__CWE135_07_good(void);
#endif /* OMITGOOD */

void run_bad(void) {
    printf("Running bad()...\n");
    CWE122_Heap_Based_Buffer_Overflow__CWE135_07_bad();
    printf("Finished running bad().\n");
}

void run_good(void) {
    printf("Running good()...\n");
    CWE122_Heap_Based_Buffer_Overflow__CWE135_07_good();
    printf("Finished running good().\n");
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