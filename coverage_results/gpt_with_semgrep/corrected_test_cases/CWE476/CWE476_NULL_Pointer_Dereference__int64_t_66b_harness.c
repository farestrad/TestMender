// ```c
#include <stdio.h>
#include <stdint.h>

void CWE476_NULL_Pointer_Dereference__int64_t_66b_badSink(int64_t * dataArray[]);
void CWE476_NULL_Pointer_Dereference__int64_t_66b_goodG2BSink(int64_t * dataArray[]);
void CWE476_NULL_Pointer_Dereference__int64_t_66b_goodB2GSink(int64_t * dataArray[]);

void run_bad(void) {
    int64_t *dataArray[5];
    int64_t *data = NULL; // Simulate bad source (NULL)
    dataArray[2] = data; // Pass NULL to sink
    printf("Running bad case...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_66b_badSink(dataArray);
}

void run_good(void) {
    int64_t *dataArray[5];
    int64_t validData = 42; // Initialize data correctly
    dataArray[2] = &validData; // Pass valid data to sink
    printf("Running good case...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_66b_goodG2BSink(dataArray);
    
    // Test the goodB2G scenario
    dataArray[2] = NULL; // Simulate bad source (NULL)
    CWE476_NULL_Pointer_Dereference__int64_t_66b_goodB2GSink(dataArray);
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