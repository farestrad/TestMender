// ```c
#include <stdio.h>
#include <stdint.h>
#include "std_testcase.h"

void CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_alloca_memmove_08_bad(void);
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_alloca_memmove_08_good(void);

void run_bad(void) {
    printf("Running bad()...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_alloca_memmove_08_bad();
}

void run_good(void) {
    printf("Running good()...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_alloca_memmove_08_good();
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