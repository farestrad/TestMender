```c
#include <stdio.h>
#include <limits.h>

/* Function prototypes from Juliet code */
void CWE190_Integer_Overflow__int_connect_socket_add_52_bad(void);
void CWE190_Integer_Overflow__int_connect_socket_add_52_good(void);

#ifndef OMITGOOD
/* Override the data source for GOOD to ensure no overflow */
int CWE190_Integer_Overflow__int_connect_socket_add_52_goodG2BSource(int data) {
    /* Set data to a small, safe, non-zero value to avoid overflow */
    return 2;
}
#endif

#ifndef OMITBAD
/* Override the data source for BAD to ensure overflow occurs */
int CWE190_Integer_Overflow__int_connect_socket_add_52_badSource(int data) {
    /* Set data to INT_MAX to provoke overflow on add */
    return INT_MAX;
}
#endif

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
```