```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Declare prototypes for the Juliet entry functions */
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_cpy_17_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_cpy_17_good(void);

/* 
 * The BAD case in Juliet does a cpy() into an undersized heap buffer.
 * To make sure this triggers an AddressSanitizer-detectable overflow,
 * we call the BAD case repeatedly to exhaust heap, and follow it with
 * an operation after the bad cpy to cause a detectable invalid access.
 *
 * Specifically, we will allocate two buffers, pass the first to the 
 * Juliet BAD function, then immediately write to the second to 
 * maximize the chance an overflow corrupts it.
 *
 * But as the Juliet testcases allocate their own memory,
 * here we simply invoke the entry point; the BAD side's copying
 * of a string without enough size should trigger ASan reliably.
 *
 * To strengthen the detection further, we force malloc to allocate 
 * small-ish blocks (by doing a leak to fill up tcache), then invoke 
 * the Juliet BAD function, to minimize the size class padding.
 */

void run_bad(void) {
    printf("Running bad case...\n");

    /* Small allocations to perturb the heap and decrease malloc padding */
    void *leak[32];
    for(int i = 0; i < 32; ++i) {
        leak[i] = malloc(20);
        memset(leak[i], 0x41, 20);
    }

    /* Now invoke the BAD function: this should copy a non-null-terminated string,
       overrunning the allocated buffer, so ASan should trigger reliably. */
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_cpy_17_bad();

    /* Force a heap touch after the overrun */
    volatile char *touch = (volatile char *)malloc(32);
    memset((void*)touch, 0x42, 32);

    /* Prevent compiler from optimizing away */
    printf("touch buffer: %02x\n", touch[0]);

    /* Clean up */
    for(int i = 0; i < 32; ++i) {
        free(leak[i]);
    }
    free((void*)touch);
}

void run_good(void) {
    printf("Running good case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_cpy_17_good();
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