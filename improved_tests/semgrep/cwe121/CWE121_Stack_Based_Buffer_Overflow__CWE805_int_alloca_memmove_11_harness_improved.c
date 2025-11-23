```c
#include "std_testcase.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>

#ifndef OMITBAD
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_11_bad(void);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_11_good(void);
#endif /* OMITGOOD */

/* 
 * The Juliet testcases allocate their own internal buffers. To make sure the
 * BAD path overflows, provide an environment (`SRC_BUFFER_SZ` and content) that
 * is much larger than the small destination buffer inside BAD. 
 *
 * Stack buffer overflows are detectable if we provoke a large memmove. 
 * We do this by manipulating global or test environment variables if allowed,
 * or by triggering code paths that copy more elements. Here we simply call 
 * the BAD and GOOD functions repeatedly to guide detection.
 */

#ifndef TEST_ITERATIONS
#define TEST_ITERATIONS 1
#endif

#ifdef _MSC_VER
#define NORETURN __declspec(noreturn)
#else
#define NORETURN __attribute__((noreturn))
#endif

/* Optionally, catch SIGABRT to flush output so crash is visible in logs. */
static jmp_buf asan_env;

void NORETURN asan_sigabrt(int sig)
{
    fflush(stdout);
    fflush(stderr);
    /* Let program terminate with abort signal as normal */
    longjmp(asan_env, 1);
}

void run_bad(void)
{
    printf("Running bad case...\n");
    fflush(stdout);
    /* 
     * For CWE121/805 BAD case, typically an under-sized stack buffer is used,
     * then a memmove copies in 100 ints (from src). Overwrite stack redzone.
     * Call multiple times if needed.
     */
    volatile int trigger = 0;
    signal(SIGABRT, asan_sigabrt);
    if (setjmp(asan_env) == 0)
    {
        for (int i = 0; i < TEST_ITERATIONS; ++i)
        {
            CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_11_bad();
            trigger++;
        }
        printf("Finished bad case (unexpectedly no crash!).\n");
    }
    else
    {
        printf("Caught abort (likely stack buffer overflow detected by ASan).\n");
        fflush(stdout);
        exit(134); /* match typical abort code */
    }
}

void run_good(void)
{
    printf("Running good case...\n");
    fflush(stdout);
    /* GOOD path must not abort or trigger ASan */
    for (int i = 0; i < TEST_ITERATIONS; ++i)
    {
        CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_11_good();
    }
    printf("Finished good case.\n");
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