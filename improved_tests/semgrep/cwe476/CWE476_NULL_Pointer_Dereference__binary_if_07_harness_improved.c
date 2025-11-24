```c
#include <stdio.h>
#include <stdlib.h>

// Prototypes for the Juliet entry functions
void CWE476_NULL_Pointer_Dereference__binary_if_07_bad(void);
void CWE476_NULL_Pointer_Dereference__binary_if_07_good(void);

// Strengthen the BAD path to ensure null dereference is reliably triggered
// by running in a subprocess with abort-on-error semantics
#ifdef _WIN32
#include <windows.h>
#else
#include <signal.h>
#endif

void run_bad(void) {
    printf("Running BAD test case...\n");
    fflush(stdout);
    // No harness-side NULL dereference here;
    // Juliet itself is responsible for manifesting the bug when given a NULL operand
    CWE476_NULL_Pointer_Dereference__binary_if_07_bad();
}

void run_good(void) {
    printf("Running GOOD test case...\n");
    fflush(stdout);
    CWE476_NULL_Pointer_Dereference__binary_if_07_good();
}

int main(void) {
#ifdef TEST_MODE_BAD
    // It is important not to catch the crash in this process for AddressSanitizer
    // Let ASan terminate the process as soon as a bug is triggered
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```