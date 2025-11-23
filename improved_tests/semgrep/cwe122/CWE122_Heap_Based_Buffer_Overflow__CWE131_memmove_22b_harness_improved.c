```c
#include <stdio.h>
#include <stdlib.h>

/* Prototypes for Juliet entry points */
void CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B1(void);
void CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B2(void);

/* 
 * The Juliet bad() and good() entry points will allocate, fill, 
 * and use heap buffers themselves. The harness does not need 
 * to set up custom inputs.
 * 
 * Ensure the GOOD path runs cleanly. If an error or crash occurs, return EXIT_FAILURE.
 * If the BAD path does NOT crash, also return EXIT_FAILURE.
 */

int main(void) {
    int result = EXIT_SUCCESS;
#ifdef TEST_MODE_BAD
    printf("Running BAD case...\n");
    fflush(stdout);
    CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_bad();
    /* If we reach here, it's an error: BAD should crash with AddressSanitizer */
    fprintf(stderr, "ERROR: BAD path did not trigger a detectable problem!\n");
    result = EXIT_FAILURE;
#else
    printf("Running GOOD case 1...\n");
    fflush(stdout);
    CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B1();

    printf("Running GOOD case 2...\n");
    fflush(stdout);
    CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B2();

    /* If we reach here, both GOOD cases ran cleanly */
    result = EXIT_SUCCESS;
#endif
    return result;
}
```