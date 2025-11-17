```c
#include <stdio.h>
#include <stdint.h>

void CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_declare_loop_54e_badSink(int64_t * data);
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_declare_loop_54e_goodG2BSink(int64_t * data);

void run_bad(void) {
    int64_t data[50]; // Create a small buffer to trigger the overflow
    printf("Running BAD test case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_declare_loop_54e_badSink(data);
}

void run_good(void) {
    int64_t data[100]; // Create a sufficiently large buffer to prevent overflow
    printf("Running GOOD test case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_declare_loop_54e_goodG2BSink(data);
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