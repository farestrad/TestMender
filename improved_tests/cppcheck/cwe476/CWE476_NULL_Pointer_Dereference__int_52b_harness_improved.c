```c
#include <stdio.h>
#include <stdlib.h>

/* Function prototypes (provided by Juliet) */
void CWE476_NULL_Pointer_Dereference__int_52b_badSink(int * data);
void CWE476_NULL_Pointer_Dereference__int_52b_goodG2BSink(int * data);
void CWE476_NULL_Pointer_Dereference__int_52b_goodB2GSink(int * data);

void run_bad(void) {
    int *data = NULL;
    printf("Running bad case...\n");
    CWE476_NULL_Pointer_Dereference__int_52b_badSink(data);
}

void run_good(void) {
    /* GOOD: data is properly initialized and non-NULL */
    int goodVal = 12345;
    int *data = &goodVal;

    printf("Running good case (goodG2B)...\n");
    CWE476_NULL_Pointer_Dereference__int_52b_goodG2BSink(data);

    printf("Running good case (goodB2G)...\n");
    /* For goodB2G, per Juliet design, a NULL should be safely handled, so we explicitly test this */
    int *nullData = NULL;
    CWE476_NULL_Pointer_Dereference__int_52b_goodB2GSink(nullData);
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