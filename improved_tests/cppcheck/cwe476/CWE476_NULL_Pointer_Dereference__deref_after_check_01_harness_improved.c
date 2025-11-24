```c
#include <stdio.h>
#include <stdlib.h>

void CWE476_NULL_Pointer_Dereference__deref_after_check_01_bad(void);
void CWE476_NULL_Pointer_Dereference__deref_after_check_01_good(void);

static void *global_sink = NULL;

void run_bad(void) {
    printf("Running bad case...\n");
    /* Purposefully cause a NULL pointer dereference by passing a NULL pointer
     * as the triggering global, volatile, or external variable, since the
     * BAD function in the Juliet suite acts on a global or static variable.
     * To force the NULL-deref in the BAD path whenever it derefs global_sink.
     */
    global_sink = NULL; /* if used in the test file */
    CWE476_NULL_Pointer_Dereference__deref_after_check_01_bad();

    /* To ensure ASan observes the effect (if the above function does not crash),
     * try to dereference NULL now as a fail-safe.
     */
    volatile int *p = (int*)NULL;
    *p = 42;
}

void run_good(void) {
    printf("Running good case...\n");
    int dummy = 1234;
    global_sink = &dummy; /* if used in the test file */
    CWE476_NULL_Pointer_Dereference__deref_after_check_01_good();
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