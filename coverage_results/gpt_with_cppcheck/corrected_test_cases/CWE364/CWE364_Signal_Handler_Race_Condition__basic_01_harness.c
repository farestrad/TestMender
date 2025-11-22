// ```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// Function prototypes for the Juliet test cases
void CWE364_Signal_Handler_Race_Condition__basic_01_bad(void);
void CWE364_Signal_Handler_Race_Condition__basic_01_good(void);

// Function to run the bad test case
void run_bad(void) {
    printf("Running BAD test case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_01_bad();
    printf("Finished BAD test case.\n");
}

// Function to run the good test case
void run_good(void) {
    printf("Running GOOD test case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_01_good();
    printf("Finished GOOD test case.\n");
}

// Main function
int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
// ```