```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// Prototypes for the Juliet entry functions
void CWE364_Signal_Handler_Race_Condition__basic_17_bad(void);
void CWE364_Signal_Handler_Race_Condition__basic_17_good(void);

// Function to run the bad test case
void run_bad(void) {
    printf("Running bad test case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_17_bad();
    printf("Finished bad test case.\n");
}

// Function to run the good test case
void run_good(void) {
    printf("Running good test case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_17_good();
    printf("Finished good test case.\n");
}

// Main function to control the test execution mode
int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```