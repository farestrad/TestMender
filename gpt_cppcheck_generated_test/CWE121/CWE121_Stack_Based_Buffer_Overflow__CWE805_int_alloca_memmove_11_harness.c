```c
#include <stdio.h>
#include "std_testcase.h"

#ifndef OMITBAD
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_11_bad(void);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_11_good(void);
#endif /* OMITGOOD */

void run_bad(void) {
    printf("Running bad case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_11_bad();
    printf("Finished running bad case.\n");
}

void run_good(void) {
    printf("Running good case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_11_good();
    printf("Finished running good case.\n");
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