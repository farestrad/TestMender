// ```c
#include <stdio.h>
#include <stdint.h>

// Declare prototypes for the Juliet entry functions
void CWE476_NULL_Pointer_Dereference__int64_t_66b_badSink(int64_t * dataArray[]);
void CWE476_NULL_Pointer_Dereference__int64_t_66b_goodG2BSink(int64_t * dataArray[]);
void CWE476_NULL_Pointer_Dereference__int64_t_66b_goodB2GSink(int64_t * dataArray[]);

// Function to run the BAD scenario
void run_bad(void) {
    int64_t *dataArray[3];
    // Setting dataArray[2] to NULL to trigger the NULL dereference
    dataArray[2] = NULL;
    printf("Running BAD scenario...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_66b_badSink(dataArray);
}

// Function to run the GOOD scenario (Good source, Bad sink)
void run_good(void) {
    int64_t value = 42; // Initialize data properly
    int64_t *dataArray[3];
    dataArray[2] = &value; // Valid pointer in dataArray[2]
    printf("Running GOOD scenario...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_66b_goodG2BSink(dataArray);
}

// Function to run the GOOD scenario (Bad source, Good sink)
void run_good_b2g(void) {
    int64_t *dataArray[3];
    // Setting dataArray[2] to NULL to demonstrate good handling
    dataArray[2] = NULL;
    printf("Running GOOD B2G scenario...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_66b_goodB2GSink(dataArray);
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
    run_good_b2g(); // Optional call to test the good handling
#endif
    return 0;
}
// ```