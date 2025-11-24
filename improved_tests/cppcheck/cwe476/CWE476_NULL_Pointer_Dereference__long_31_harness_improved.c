```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void CWE476_NULL_Pointer_Dereference__long_31_bad(void);
void CWE476_NULL_Pointer_Dereference__long_31_good(void);

static void sigsegv_handler(int signo) {
    fprintf(stderr, "Caught SIGSEGV (likely NULL pointer dereference)\n");
    exit(134); // mimic abort signal status to keep exit code behavior
}

void run_bad(void) {
    printf("Running BAD test case...\n");
    /* Install handler to ensure segfault signals cause abnormal exit */
    signal(SIGSEGV, sigsegv_handler);
    CWE476_NULL_Pointer_Dereference__long_31_bad();
}

void run_good(void) {
    printf("Running GOOD test case...\n");
    CWE476_NULL_Pointer_Dereference__long_31_good();
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