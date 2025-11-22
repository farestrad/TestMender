// ```c
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef OMITBAD
void CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_ncat_41_bad(void);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_ncat_41_good(void);
#endif /* OMITGOOD */

void run_bad(void) {
    printf("Running bad test case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_ncat_41_bad();
    printf("Finished running bad test case.\n");
}

void run_good(void) {
    printf("Running good test case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_ncat_41_good();
    printf("Finished running good test case.\n");
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