// ```c
#include <stdio.h>
#include <wchar.h>

// Prototypes for the Juliet entry functions
void CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_declare_memcpy_15_bad(void);
void CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_declare_memcpy_15_good(void);

void run_bad(void) {
    printf("Running BAD test case...\n");
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
// ```