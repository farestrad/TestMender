```c
#include <stdint.h>
#include <stdio.h>

void CWE476_NULL_Pointer_Dereference__int64_t_54c_badSink(int64_t * data);
void CWE476_NULL_Pointer_Dereference__int64_t_54c_goodG2BSink(int64_t * data);

void run_bad(void) {
    int64_t *data = NULL; // BAD source triggers null dereference
    printf("Running BAD test case...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_54c_badSink(data);
}

void run_good(void) {
    int64_t goodValue = 123456789;
    int64_t *data = &goodValue; // GOOD: properly initialized pointer
    printf("Running GOOD test case...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_54c_goodG2BSink(data);
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