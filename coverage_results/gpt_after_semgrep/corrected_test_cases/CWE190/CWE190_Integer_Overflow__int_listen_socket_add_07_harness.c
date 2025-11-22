// ```c
#include "std_testcase.h"

#ifndef OMITBAD
void CWE190_Integer_Overflow__int_listen_socket_add_07_bad(void);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE190_Integer_Overflow__int_listen_socket_add_07_good(void);
#endif /* OMITGOOD */

void run_bad(void) {
    printf("Running bad case...\n");
    CWE190_Integer_Overflow__int_listen_socket_add_07_bad();
}

void run_good(void) {
    printf("Running good cases...\n");
    CWE190_Integer_Overflow__int_listen_socket_add_07_good();
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