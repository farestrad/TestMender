// ```c
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

void CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_memcpy_11_bad(void);
void CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_memcpy_11_good(void);

void run_bad(void) {
    printf("Running bad case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_memcpy_11_bad();
    printf("Finished bad case.\n");
}

void run_good(void) {
    printf("Running good case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_memcpy_11_good();
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
// ```