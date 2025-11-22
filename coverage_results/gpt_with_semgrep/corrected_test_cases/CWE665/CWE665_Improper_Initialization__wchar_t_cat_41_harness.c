// ```c
#include <wchar.h>
#include <stdio.h>

void CWE665_Improper_Initialization__wchar_t_cat_41_bad(void);
void CWE665_Improper_Initialization__wchar_t_cat_41_good(void);

void run_bad(void) {
    wprintf(L"Running BAD case...\n");
    CWE665_Improper_Initialization__wchar_t_cat_41_bad();
}

void run_good(void) {
    wprintf(L"Running GOOD case...\n");
    CWE665_Improper_Initialization__wchar_t_cat_41_good();
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