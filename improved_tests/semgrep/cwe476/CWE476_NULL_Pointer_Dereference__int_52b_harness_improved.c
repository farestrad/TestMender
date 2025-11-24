```c
#include <stdio.h>

// Prototypes for the Juliet entry functions
void CWE476_NULL_Pointer_Dereference__int_52c_badSink(int * data);
void CWE476_NULL_Pointer_Dereference__int_52c_goodG2BSink(int * data);
void CWE476_NULL_Pointer_Dereference__int_52c_goodB2GSink(int * data);

void run_bad(void) {
    int *data = NULL;
    printf("Running BAD test with NULL pointer...\n");
    CWE476_NULL_Pointer_Dereference__int_52c_badSink(data);
}

void run_good(void) {
    int value = 100;
    int *data = &value;
    printf("Running GOOD test with initialized pointer...\n");

    // Call both goodG2B and goodB2G paths if available
    CWE476_NULL_Pointer_Dereference__int_52c_goodG2BSink(data);
    CWE476_NULL_Pointer_Dereference__int_52c_goodB2GSink(data);
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