```c
#include <stdint.h>
#include <stdio.h>
#include "std_testcase.h"

// Declare prototypes for the Juliet entry functions
void CWE476_NULL_Pointer_Dereference__int64_t_51b_badSink(int64_t * data);
void CWE476_NULL_Pointer_Dereference__int64_t_51b_goodG2BSink(int64_t * data);
void CWE476_NULL_Pointer_Dereference__int64_t_51b_goodB2GSink(int64_t * data);

// BAD harness: trigger NULL dereference
void run_bad(void) {
    int64_t *data = NULL;
    printf("Running BAD case...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_51b_badSink(data);
}

// GOOD harness: supply valid pointer
void run_good(void) {
    int64_t value = 123456789013579LL;
    int64_t *data = &value;
    printf("Running GOOD case...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_51b_goodG2BSink(data);
}

// GOOD B2G harness: NULL pointer, which should be handled safely
void run_good_B2G(void) {
    int64_t *data = NULL;
    printf("Running GOOD B2G case...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_51b_goodB2GSink(data);
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#elif defined(TEST_MODE_GOODB2G)
    run_good_B2G();
#else
    run_good();
#endif
    return 0;
}
```