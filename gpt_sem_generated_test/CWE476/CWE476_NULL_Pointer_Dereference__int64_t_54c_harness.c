```c
#include <stdio.h>
#include <stdint.h>

void CWE476_NULL_Pointer_Dereference__int64_t_54c_badSink(int64_t * data);
void CWE476_NULL_Pointer_Dereference__int64_t_54c_goodG2BSink(int64_t * data);
void CWE476_NULL_Pointer_Dereference__int64_t_54c_goodB2GSink(int64_t * data);

void run_bad(void) {
    int64_t *data = NULL; // Set data to NULL to trigger the vulnerability
    printf("Running bad case - invoking badSink with NULL data...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_54c_badSink(data);
}

void run_good(void) {
    int64_t value = 42; // Initialize data
    int64_t *data = &value; // Pointing to a valid memory location
    printf("Running good case - invoking goodG2BSink with initialized data...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_54c_goodG2BSink(data);
    
    printf("Running good case - invoking goodB2GSink with NULL data...\n");
    data = NULL; // Set data to NULL to demonstrate good handling
    CWE476_NULL_Pointer_Dereference__int64_t_54c_goodB2GSink(data);
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