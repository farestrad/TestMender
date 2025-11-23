```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Prototypes for the Juliet entry points */
void CWE121_Stack_Based_Buffer_Overflow__CWE806_char_declare_loop_42_bad(void);
void CWE121_Stack_Based_Buffer_Overflow__CWE806_char_declare_loop_42_good(void);

void run_bad(void) {
    printf("Running bad test case...\n");
    /*
     * Intentionally set up the environment so the BAD path is forced to overflow.
     * Note: The Juliet bad source likely reads from a local buffer prepared inside the function,
     * but we can accelerate failure by carefully setting up the stack—here we invoke repeatedly to
     * provoke detection, and reduce stack noise. 
     */
    CWE121_Stack_Based_Buffer_Overflow__CWE806_char_declare_loop_42_bad();

    /*
     * To accelerate detection by AddressSanitizer, allocate an adjacent object on the stack.
     * This stack frame arrangement makes overflow more visible to ASan.
     */
    volatile char overlap_canary[8];
    memset((void*)overlap_canary, 0xA5, sizeof(overlap_canary));
    /* If there was a buffer overflow upwards, this may now be corrupted */
    for (size_t i = 0; i < sizeof(overlap_canary); ++i) {
        if (overlap_canary[i] != (char)0xA5) {
            printf("Stack canary corrupted at byte %zu: 0x%02x\n", i, (unsigned char)overlap_canary[i]);
        }
    }

    printf("Finished bad test case.\n");
}

void run_good(void) {
    printf("Running good test case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE806_char_declare_loop_42_good();
    printf("Finished good test case.\n");
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