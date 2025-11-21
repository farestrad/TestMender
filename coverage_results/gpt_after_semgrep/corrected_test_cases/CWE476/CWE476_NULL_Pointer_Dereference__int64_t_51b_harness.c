// ```c
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
    int64_t value = 10; // Good source: initializing data
    int64_t *data = &value;
    printf("Running good case: Initialized pointer to a valid memory address...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_51b_goodG2BSink(data);

    data = NULL; // Bad source case for goodB2G
    printf("Running goodB2G case: Pointer is NULL...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_51b_goodB2GSink(data);
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