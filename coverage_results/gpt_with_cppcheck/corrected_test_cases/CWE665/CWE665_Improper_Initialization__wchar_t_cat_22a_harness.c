// ```c
#include <stdio.h>
#include <wchar.h>

void CWE665_Improper_Initialization__wchar_t_cat_22_bad(void);
void CWE665_Improper_Initialization__wchar_t_cat_22_good(void);

void run_bad(void) {
    printf("Running bad test case...\n");
    CWE665_Improper_Initialization__wchar_t_cat_22_bad();
    printf("Finished bad test case.\n");
}

void run_good(void) {
    printf("Running good test case...\n");
    CWE665_Improper_Initialization__wchar_t_cat_22_good();
    printf("Finished good test case.\n");
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