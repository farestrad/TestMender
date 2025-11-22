// ```c
#include "std_testcase.h"

#ifndef OMITBAD
void CWE121_Stack_Based_Buffer_Overflow__CWE129_connect_socket_05_bad(void);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE121_Stack_Based_Buffer_Overflow__CWE129_connect_socket_05_good(void);
#endif /* OMITGOOD */

void run_bad(void)
{
    printf("Running bad test case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE129_connect_socket_05_bad();
}

void run_good(void)
{
    printf("Running good test case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE129_connect_socket_05_good();
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