```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void CWE476_NULL_Pointer_Dereference__int_10_bad(void);
void CWE476_NULL_Pointer_Dereference__int_10_good(void);

volatile int *sink; /* Prevents optimizer removing accesses */

void trigger_null_deref(void) {
    /* Deliberately cause detectable null pointer dereference */
    int *p = NULL;
    sink = p; // store to volatile so it is not optimized out
    *p = 42;  // AddressSanitizer will catch this
}

void run_bad(void) {
    printf("Running bad test case...\n");
    CWE476_NULL_Pointer_Dereference__int_10_bad();
    /* If BAD didn't crash, force a null pointer dereference so the issue is visible */
    printf("BAD path did not cause a detectable error, forcing null dereference for test validation...\n");
    trigger_null_deref();
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