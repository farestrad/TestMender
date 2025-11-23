```c
#include <stdio.h>
#include <stdlib.h>

/* Juliet entry point declarations */
void CWE476_NULL_Pointer_Dereference__int_54b_badSink(int * data);
void CWE476_NULL_Pointer_Dereference__int_54b_goodG2BSink(int * data);
void CWE476_NULL_Pointer_Dereference__int_54b_goodB2GSink(int * data);

void run_bad(void) {
    int *data = NULL; // BadSource: Set data to NULL
    printf("Running bad test case...\n");
    CWE476_NULL_Pointer_Dereference__int_54b_badSink(data); // Expected to trigger NULL dereference
}

void run_good(void) {
    /* GOOD test for goodG2B: data initialized and valid */
    int good_val = 42;
    int *data = &good_val;
    printf("Running goodG2B test case...\n");
    CWE476_NULL_Pointer_Dereference__int_54b_goodG2BSink(data); // Should NOT crash

    /* GOODB2G: Even though data is NULL, path should check for NULL before dereferencing */
    data = NULL;
    printf("Running goodB2G test case...\n");
    CWE476_NULL_Pointer_Dereference__int_54b_goodB2GSink(data); // Should safely handle NULL and NOT crash
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