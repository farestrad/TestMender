```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void CWE476_NULL_Pointer_Dereference__int_10_bad(void);
void CWE476_NULL_Pointer_Dereference__int_10_good(void);

/* 
 * Ensure that a crash in the BAD case does not produce an infinite loop if 
 * signal is ignored; but AddressSanitizer should still detect the bug. 
 * This handler is just for a clean abort fallback.
 */
void sigsegv_handler(int sig) {
    fprintf(stderr, "Segmentation fault caught\n");
    exit(134);
}

void run_bad(void) {
    printf("Running bad test case...\n");
    /* Remove ambiguity: ensure that a NULL dereference is NOT elided by compiler/linker optimizations */
    /* Avoid catching and masking the error except as a fallback for non-ASAN runs */
    signal(SIGSEGV, sigsegv_handler);
    CWE476_NULL_Pointer_Dereference__int_10_bad();
}

void run_good(void) {
    printf("Running good test case...\n");
    CWE476_NULL_Pointer_Dereference__int_10_good();
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