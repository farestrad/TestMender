// ```c
#include <stdio.h>
#include <stdlib.h>

// Prototypes for the Juliet test cases
void CWE122_Heap_Based_Buffer_Overflow__CWE131_loop_21_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__CWE131_loop_21_good(void);

void run_bad(void) {
    printf("Running BAD case...\n");
    CWE122_Heap_Based_Buffer_Overflow__CWE131_loop_21_bad();
}

void run_good(void) {
    printf("Running GOOD case...\n");
    CWE122_Heap_Based_Buffer_Overflow__CWE131_loop_21_good();
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
// ```