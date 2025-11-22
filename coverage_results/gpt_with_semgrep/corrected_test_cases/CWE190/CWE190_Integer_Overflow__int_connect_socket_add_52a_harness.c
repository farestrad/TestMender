// ```c
#include <stdio.h>

/* Function prototypes */
void CWE190_Integer_Overflow__int_connect_socket_add_52_bad(void);
void CWE190_Integer_Overflow__int_connect_socket_add_52_good(void);

void run_bad(void) {
    printf("Running BAD test...\n");
    CWE190_Integer_Overflow__int_connect_socket_add_52_bad();
}

void run_good(void) {
    printf("Running GOOD test...\n");
    CWE190_Integer_Overflow__int_connect_socket_add_52_good();
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