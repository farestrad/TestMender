```c
#include <stdio.h>
#include <stdlib.h>
#include "std_testcase.h"

#ifndef OMITBAD
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_54b_badSink(int * data);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_54b_goodG2BSink(int * data);
#endif /* OMITGOOD */

void run_bad(void)
{
    int *data;
    /* FLAW: Allocate a small buffer on the stack using alloca */
    data = (int *)ALLOCA(10 * sizeof(int));
    /* Pass data to the next function, which will eventually trigger the overflow */
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_54b_badSink(data);
    printf("Executed run_bad()\n");
}

void run_good(void)
{
    int *data;
    /* FIX: Allocate a large enough buffer on the stack using alloca */
    data = (int *)ALLOCA(20 * sizeof(int));
    /* Pass data to the next function, which will handle it safely */
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_54b_goodG2BSink(data);
    printf("Executed run_good()\n");
}

int main(void)
{
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```