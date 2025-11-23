```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Prototypes for the Juliet functions */
void CWE122_Heap_Based_Buffer_Overflow__c_CWE806_char_snprintf_13_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE806_char_snprintf_13_good(void);

/*
 * The key to reliably triggering a heap overflow in the Juliet testcase is to
 * control the global control flags (if any are used) and/or the environment,
 * but primarily to ensure the BAD path receives input large enough to
 * overflow, and the GOOD does not.
 *
 * These entrypoints allocate their own buffers, but both depend on the
 * global variable 'globalFive' to choose between BAD and GOOD source sizes.
 * Therefore, setting globalFive to 5 is enough to make the 'bad' path flow.
 * Additionally, to emphasize overflows, we can call the function in a child
 * process (optional), but for simple Juliet harnesses we just invoke.
 */

#ifdef __cplusplus
extern "C" {
#endif

/* This variable is used in the Juliet "13" variant (globalFive is compared to 5) */
extern int globalFive;

#ifdef __cplusplus
}
#endif

void run_bad(void) {
    printf("Running BAD test...\n");
    /* Force maximum control flow for BAD path */
    globalFive = 5;
    CWE122_Heap_Based_Buffer_Overflow__c_CWE806_char_snprintf_13_bad();
}

void run_good(void) {
    printf("Running GOOD test...\n");
    /* Force maximum control flow for GOOD path */
    globalFive = 5;
    CWE122_Heap_Based_Buffer_Overflow__c_CWE806_char_snprintf_13_good();
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