// ```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// Prototypes for the original Juliet entry functions
void CWE364_Signal_Handler_Race_Condition__basic_14_bad(void);
void CWE364_Signal_Handler_Race_Condition__basic_14_good(void);

// Functions to run tests
void run_bad(void) {
    printf("Running bad test...\n");
    CWE364_Signal_Handler_Race_Condition__basic_14_bad();
    printf("Finished bad test.\n");
}

void run_good(void) {
    printf("Running good test...\n");
    CWE364_Signal_Handler_Race_Condition__basic_14_good();
    printf("Finished good test.\n");
}

// Main function to control the test execution
int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
// ```