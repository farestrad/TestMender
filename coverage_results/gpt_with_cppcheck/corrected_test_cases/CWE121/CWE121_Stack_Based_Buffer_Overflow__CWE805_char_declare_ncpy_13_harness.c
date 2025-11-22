// ```c
#include <stdio.h>
#include <string.h>

void CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncpy_13_bad(void);
void CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncpy_13_good(void);

void run_bad(void) {
    printf("Running bad test case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncpy_13_bad();
}

void run_good(void) {
    printf("Running good test case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncpy_13_good();
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