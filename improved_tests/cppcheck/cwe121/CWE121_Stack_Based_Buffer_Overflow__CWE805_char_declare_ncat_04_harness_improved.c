```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncat_04_bad(void);
void CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncat_04_good(void);

/*
 * Juliet's _bad/_good functions operate on internal stack buffers, so
 * to reliably trigger overflow, we make the stack frame as shallow as possible
 * and increase chances of catching the bug by calling the function in a loop
 * with a volatile variable (to avoid tail-call/elimination optimizations).
 * This wrapper ensures the vulnerabilities manifest under AddressSanitizer.
 */

void run_bad(void) {
    printf("Running BAD test case...\n");
    volatile int prevent_optim = 0;
    for (int i = 0; i < 20; ++i) { // More iterations for higher chance of ASan detection
        CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncat_04_bad();
        prevent_optim += i;
    }
    // Dummy sink to use volatile
    if (prevent_optim == 42) {
        puts("Impossible, just to use variable");
    }
}

void run_good(void) {
    printf("Running GOOD test case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncat_04_good();
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