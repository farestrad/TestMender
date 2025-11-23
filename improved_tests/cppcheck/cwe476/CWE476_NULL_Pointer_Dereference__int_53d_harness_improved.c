```c
#include <stdio.h>

// Declare prototypes for the Juliet entry functions
void CWE476_NULL_Pointer_Dereference__int_53d_badSink(int *data);
void CWE476_NULL_Pointer_Dereference__int_53d_goodG2BSink(int *data);
void CWE476_NULL_Pointer_Dereference__int_53d_goodB2GSink(int *data);

// BAD path: supply NULL pointer to trigger null deref in sink
void run_bad(void) {
    int *data = NULL;
    printf("Running BAD test case...\n");
    CWE476_NULL_Pointer_Dereference__int_53d_badSink(data);
}

// GOOD G2B path: supply a valid pointer value to goodG2B sink
void run_good(void) {
    int safe_data = 4242;
    int *data = &safe_data;
    printf("Running GOOD G2B test case...\n");
    CWE476_NULL_Pointer_Dereference__int_53d_goodG2BSink(data);
}

// GOOD B2G path: supply NULL pointer, but sink must handle NULL safely
void run_good_b2g(void) {
    int *data = NULL;
    printf("Running GOOD B2G test case...\n");
    CWE476_NULL_Pointer_Dereference__int_53d_goodB2GSink(data);
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
    run_good_b2g();
#endif
    return 0;
}
```