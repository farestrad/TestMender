```c
#include <wchar.h>
#include "std_testcase.h"

void CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_declare_ncpy_07_bad(void);
void CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_declare_ncpy_07_good(void);

void run_bad(void) {
    wprintf(L"Running bad function...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_declare_ncpy_07_bad();
}

void run_good(void) {
    wprintf(L"Running good functions...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_declare_ncpy_07_good();
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