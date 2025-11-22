// ```c
#include <stdio.h>

// Declare prototypes for the Juliet entry functions
void CWE476_NULL_Pointer_Dereference__int_52c_badSink(int * data);
void CWE476_NULL_Pointer_Dereference__int_52c_goodG2BSink(int * data);
void CWE476_NULL_Pointer_Dereference__int_52c_goodB2GSink(int * data);

void run_bad(void) {
    int *data;
    // Bad source: Set data to NULL
    data = NULL;
    printf("Running BAD test with NULL pointer...\n");
    // Call the bad sink which will cause the NULL dereference issue
    CWE476_NULL_Pointer_Dereference__int_52c_badSink(data);
}

void run_good(void) {
    int value = 42;
    int *data;
    // Good source: Initialize data
    data = &value;
    printf("Running GOOD test with initialized pointer...\n");
    // Call the good sink which checks for NULL before dereferencing
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
// ```