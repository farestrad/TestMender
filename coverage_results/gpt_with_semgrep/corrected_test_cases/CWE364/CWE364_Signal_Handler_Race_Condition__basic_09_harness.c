// ```c
#include <stdio.h>
#include <stdlib.h>

// Prototypes for the Juliet entry functions
void CWE364_Signal_Handler_Race_Condition__basic_09_bad(void);
void CWE364_Signal_Handler_Race_Condition__basic_09_good(void);

// Function to drive the BAD test case
void run_bad(void) {
    printf("Running BAD test case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_09_bad();
    printf("Finished BAD test case.\n");
}

// Function to drive the GOOD test case
void run_good(void) {
    printf("Running GOOD test case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_09_good();
    printf("Finished GOOD test case.\n");
}

// Main function to execute the appropriate test path
int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
// ```