#include "std_testcase.h"

#ifndef OMITBAD
void CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_alloca_memmove_67_bad(void);
#endif

#ifndef OMITGOOD
void CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_alloca_memmove_67_good(void);
#endif

void run_bad(void) {
    CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_alloca_memmove_67_bad();
}

void run_good(void) {
    CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_alloca_memmove_67_good();
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
