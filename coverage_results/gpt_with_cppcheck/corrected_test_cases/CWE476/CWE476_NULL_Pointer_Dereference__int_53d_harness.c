// ```c
#include <stdio.h>

// Declare prototypes for the Juliet entry functions
void CWE476_NULL_Pointer_Dereference__int_53d_badSink(int *data);
void CWE476_NULL_Pointer_Dereference__int_53d_goodG2BSink(int *data);
void CWE476_NULL_Pointer_Dereference__int_53d_goodB2GSink(int *data);

// Function to run the BAD test case
void run_bad(void) {
    int *data = NULL; // Simulate the BAD source by setting data to NULL
    printf("Running BAD test case...\n");
    CWE476_NULL_Pointer_Dereference__int_53d_badSink(data); // Call the BAD sink
}

// Function to run the GOOD test case (Good Source with Bad Sink)
void run_good(void) {
    int value = 42; // Proper initialization of data
    int *data = &value; // Point data to a valid address
    printf("Running GOOD test case...\n");
    CWE476_NULL_Pointer_Dereference__int_53d_goodG2BSink(data); // Call GOOD sink with initialized data
}

// Function to run the GOOD test case (Bad Source with Good Sink)
void run_good_b2g(void) {
    int *data = NULL; // Simulate the BAD source
    printf("Running GOOD B2G test case...\n");
    CWE476_NULL_Pointer_Dereference__int_53d_goodB2GSink(data); // Call the GOOD sink which checks for NULL
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
// ```