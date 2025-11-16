```c
#include <stdio.h>
#include <string.h>
#include "std_testcase.h"

#ifndef OMITBAD
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_54c_badSink(int * data);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_54c_goodG2BSink(int * data);
#endif /* OMITGOOD */

void run_bad(void) {
    int *data;
    /* BAD: Allocate a small buffer, but try to copy more data than it can hold */
    data = (int *)alloca(10 * sizeof(int)); // Small buffer
    int largeBuffer[20] = {0}; // Large buffer

    /* Fill the large buffer with data */
    for (int i = 0; i < 20; i++) {
        largeBuffer[i] = i;
    }

    /* This will cause a buffer overflow */
    memmove(data, largeBuffer, 20 * sizeof(int)); 

    printf("Executed run_bad()\n");
}

void run_good(void) {
    int *data;
    /* GOOD: Allocate a sufficient buffer */
    data = (int *)alloca(20 * sizeof(int)); // Adequate size buffer
    int largerBuffer[20] = {0}; // Large buffer

    /* Fill the larger buffer with data */
    for (int i = 0; i < 20; i++) {
        largerBuffer[i] = i;
    }

    /* Safe copy, no overflow */
    memmove(data, largerBuffer, 20 * sizeof(int)); 

    printf("Executed run_good()\n");
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