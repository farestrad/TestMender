```c
#include "std_testcase.h"

#include <wchar.h>

void CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_memmove_15_bad(void);
void CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_memmove_15_good(void);

void run_bad(void) {
    wprintf(L"Running BAD test case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_memmove_15_bad();
    wprintf(L"Finished BAD test case.\n");
}

void run_good(void) {
    wprintf(L"Running GOOD test case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_memmove_15_good();
    wprintf(L"Finished GOOD test case.\n");
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