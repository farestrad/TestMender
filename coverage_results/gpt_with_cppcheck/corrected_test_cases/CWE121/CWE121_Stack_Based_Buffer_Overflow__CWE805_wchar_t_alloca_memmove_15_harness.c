// ```c
#include <stdio.h>
#include <wchar.h>

void CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_memmove_15_bad(void);
void CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_memmove_15_good(void);

void run_bad(void) {
    printf("Running bad case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_memmove_15_bad();
}

void run_good(void) {
    printf("Running good case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_memmove_15_good();
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