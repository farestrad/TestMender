```c
#include <stdio.h>
#include <stdlib.h>

/* Function prototypes */
void CWE476_NULL_Pointer_Dereference__int_52b_badSink(int * data);
void CWE476_NULL_Pointer_Dereference__int_52b_goodG2BSink(int * data);
void CWE476_NULL_Pointer_Dereference__int_52b_goodB2GSink(int * data);

void run_bad(void) {
    int *data = NULL; // Simulating the NULL pointer dereference
    printf("Running bad case...\n");
    CWE476_NULL_Pointer_Dereference__int_52b_badSink(data); // This will lead to a NULL dereference
}

void run_good(void) {
    int val = 42; // Proper initialization
    int *data = &val; // Avoiding NULL
    printf("Running good case...\n");
    CWE476_NULL_Pointer_Dereference__int_52b_goodG2BSink(data); // Safe path
    CWE476_NULL_Pointer_Dereference__int_52b_goodB2GSink(data); // Using the good sink
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