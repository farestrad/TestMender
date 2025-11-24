```c
#include <stdio.h>
#include <stdint.h>

// Prototypes for the Juliet entry functions
void CWE476_NULL_Pointer_Dereference__int64_t_51b_badSink(int64_t * data);
void CWE476_NULL_Pointer_Dereference__int64_t_51b_goodG2BSink(int64_t * data);
void CWE476_NULL_Pointer_Dereference__int64_t_51b_goodB2GSink(int64_t * data);

void run_bad(void) {
    int64_t *data = NULL; // Bad source: setting data to NULL
    printf("Running bad case: Attempting to dereference a NULL pointer...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_51b_badSink(data);
}

void run_good(void) {
    // GoodG2B: data initialized to a valid int64_t
    int64_t good_data = 123456789012345LL;
    int64_t *data = &good_data;
    printf("Running goodG2B case: Pointer to valid int64_t...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_51b_goodG2BSink(data);

    // GoodB2G: data set to NULL, but goodB2G should be safe
    int64_t *nulldata = NULL;
    printf("Running goodB2G case: Pointer is NULL, sink should handle safely...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_51b_goodB2GSink(nulldata);
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