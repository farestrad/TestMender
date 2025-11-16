```c
#include <stdio.h>
#include <stdlib.h>

void CWE476_NULL_Pointer_Dereference__int_10_bad(void);
void CWE476_NULL_Pointer_Dereference__int_10_good(void);

void run_bad(void) {
    printf("Running bad test case...\n");
    CWE476_NULL_Pointer_Dereference__int_10_bad();
}

void run_good(void) {
    printf("Running good test case...\n");
    CWE476_NULL_Pointer_Dereference__int_10_good();
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