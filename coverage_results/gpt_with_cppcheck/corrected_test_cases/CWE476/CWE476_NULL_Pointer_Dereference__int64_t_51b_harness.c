// ```c
#include <stdint.h>
#include <stdio.h>
#include "std_testcase.h"

// Declare prototypes for the Juliet entry functions
void CWE476_NULL_Pointer_Dereference__int64_t_51b_badSink(int64_t * data);
void CWE476_NULL_Pointer_Dereference__int64_t_51b_goodG2BSink(int64_t * data);
void CWE476_NULL_Pointer_Dereference__int64_t_51b_goodB2GSink(int64_t * data);

// Function to test the BAD case
void run_bad(void) {
    int64_t *data = NULL; // Set data to NULL
    printf("Running BAD case...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_51b_badSink(data); // Call the BAD sink
}

// Function to test the GOOD case using Good Source with Bad Sink
void run_good(void) {
    int64_t value = 42; // Proper initialization
    int64_t *data = &value; // Use a valid pointer
    printf("Running GOOD case...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_51b_goodG2BSink(data); // Call the GOOD sink
}

// Function to test the GOOD case using Bad Source with Good Sink
void run_good_B2G(void) {
    int64_t *data = NULL; // Set data to NULL to test the Good Sink
    printf("Running GOOD B2G case...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_51b_goodB2GSink(data); // Call the GOOD B2G sink
}

// Main entry point
int main(void) {
    #ifdef TEST_MODE_BAD
        run_bad();
    #else
        run_good(); // Change to run_good_B2G() for the Good B2G case
    #endif
    return 0;
}
// ```