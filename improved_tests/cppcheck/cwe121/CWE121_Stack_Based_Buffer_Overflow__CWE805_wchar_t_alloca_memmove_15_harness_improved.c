
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>

void CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_memmove_15_bad(void);
void CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_memmove_15_good(void);

/*
 * Ensure the stack is clean so that the stack-based buffer overflow
 * is not shadowed by adjacent allocations or stack variables.
 *
 * Also, call the functions in a way that encourages buffer overflow:
 * - For BAD, repeat the call in a loop to reliably trigger the error.
 *   (Juliet bad cases typically do not crash on first run)
 * - For GOOD, run only once and keep everything minimal.
 */

void run_bad(void) {
    wprintf(L"Running bad case...\n");
    // Run multiple times to trigger ASan: if the overflow is off-by-one or NUL, repetition can help
    for (int i = 0; i < 10; ++i) {
        CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_memmove_15_bad();
    }
    // Just in case, call some memory barrier
    fflush(stdout);
}

void run_good(void) {
    wprintf(L"Running good case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_memmove_15_good();
    fflush(stdout);
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}