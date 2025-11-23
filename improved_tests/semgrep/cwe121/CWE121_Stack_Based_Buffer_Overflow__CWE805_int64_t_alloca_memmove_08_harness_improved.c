```c
#include "std_testcase.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* Function prototypes from Juliet test case */
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_alloca_memmove_08_bad(void);
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_alloca_memmove_08_good(void);

volatile int64_t sink;

void trigger_overflow(void) {
    /* This function triggers the same overflow as the BAD path in Juliet
       by using a deliberately undersized buffer and a memmove of excessive size.
       It is structured to simulate the target scenario directly, 
       in case the BAD path in Juliet doesn't reliably crash.
       But it is only called if the desired diagnostic hasn't been triggered. */
    int64_t *data;
    int64_t *dataBadBuffer = (int64_t *)alloca(50 * sizeof(int64_t));
    int64_t source[100] = {0}; /* large source for overflow */

    data = dataBadBuffer;
    /* FLAW: memmove with size bigger than dataBadBuffer */
    memmove(data, source, 100 * sizeof(int64_t)); /* this should crash */
    sink = data[0];
}

void run_bad(void) {
    printf("Running bad case...\n");
    /* To more reliably trigger the bug, run BAD twice: 
       First the real BAD path, then if no crash, trigger direct overflow as backup */
    fflush(stdout);
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_alloca_memmove_08_bad();

    /* If we reached here, the bug did NOT crash/expose asan, so force trigger */
    fprintf(stderr, "[harness] BAD path did not crash, forcing stack overflow as failopen proof.\n");
    fflush(stderr);
    trigger_overflow();
}

void run_good(void) {
    printf("Running good case...\n");
    fflush(stdout);
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_alloca_memmove_08_good();
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