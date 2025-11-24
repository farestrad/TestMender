```c
#include "std_testcase.h"
#include <stdio.h>
#include <string.h>

/* Function prototypes from Juliet testcases */
void CWE121_Stack_Based_Buffer_Overflow__CWE806_char_declare_loop_42_bad(void);
void CWE121_Stack_Based_Buffer_Overflow__CWE806_char_declare_loop_42_good(void);

/*
 * In Juliet 42-variant, data source function is usually static and local,
 * but the main sink is in the entrypoints above.
 * To help provoke the overflow in BAD, we call the BAD function as is.
 * We do not need to pass parameters, but the BAD path in the testcase will use
 * an intentionally oversized initial buffer. We ensure the environment is clean.
 */

/* Optionally clean stack before/after for ASan clarity */
static volatile char dummy_sink;

void touch_stack(void) {
    volatile char buffer[256];
    memset((void*)buffer, 'X', sizeof(buffer));
    dummy_sink = buffer[0];
}

void run_bad(void) {
    printf("Running bad case...\n");
    /* Optional to touch stack to help with ASan; not strictly required */
    touch_stack();
    CWE121_Stack_Based_Buffer_Overflow__CWE806_char_declare_loop_42_bad();
    printf("Finished bad case (should not reach here if overflow occurs).\n");
}

void run_good(void) {
    printf("Running good case...\n");
    touch_stack();
    CWE121_Stack_Based_Buffer_Overflow__CWE806_char_declare_loop_42_good();
    printf("Finished good case.\n");
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