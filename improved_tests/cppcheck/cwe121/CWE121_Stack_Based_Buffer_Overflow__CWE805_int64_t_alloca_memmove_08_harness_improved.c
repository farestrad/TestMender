```c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "std_testcase.h"

/* Declarations for the Juliet entry points */
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_alloca_memmove_08_bad(void);
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_alloca_memmove_08_good(void);

/*
 * The below #define and extern setup are for controlling the 08 "staticReturnsTrue"/"staticReturnsFalse"
 * style Juliet testcases. These testcases use static functions or global constants/variables to 
 * select the control flow, which can make it hard to directly influence the bug site.
 * Since the function names and buffer initializations are fixed in the original Juliet sources,
 * the harness's best leverage is to ensure that the entry point is called in a way that
 * maximizes buffer overflow (BAD) or ensures safety (GOOD).
 */

/* Use volatile and EXIT immediately after BAD to maximize ASan visibility before stack unwinding */
void run_bad(void) {
    printf("Running bad()...\n");
    fflush(stdout);

    /* 
     * The BAD function allocates a small buffer with alloca, then memmoves more than its size.
     * Some test environments need a call pattern to force inlining or not;
     * To maximize visibility, call BAD and then quickly EXIT (to avoid cleanup masking the crash).
     */
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_alloca_memmove_08_bad();

    /*
     * If the bug is not manifest (should not happen), force a failure.
     * But the buffer overflow should abort first.
     */
    fprintf(stderr, "ERROR: BAD path did not crash as expected\n");
    exit(1);
}

void run_good(void) {
    printf("Running good()...\n");
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