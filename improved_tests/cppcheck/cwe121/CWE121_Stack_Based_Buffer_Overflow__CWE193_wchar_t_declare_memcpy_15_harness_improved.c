
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>

// Prototypes for the Juliet entry functions
void CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_declare_memcpy_15_bad(void);
void CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_declare_memcpy_15_good(void);

volatile int force_overflow = 0;

void run_bad(void) {
    printf("Running BAD test case...\n");

    /*
     * Attempt to ensure secondary overflow by dirtying stack around where Juliet
     * would allocate buffers, to increase odds of crash/ASan detection.
     * BAD path will copy 10 wide chars into a buffer of exactly 10 without space for NULL.
     * Here, we introduce a wider pattern around stack frame.
     */

    wchar_t dummy[8];
    for(int i = 0; i < 8; i++)
        dummy[i] = L'A';

    // Optionally, force the compiler not to optimize away dummy array
    if (force_overflow)
        dummy[0] = L'B';

    CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_declare_memcpy_15_bad();

    printf("Finished BAD test case.\n");
}

void run_good(void) {
    printf("Running GOOD test case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_declare_memcpy_15_good();
    printf("Finished GOOD test case.\n");
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}