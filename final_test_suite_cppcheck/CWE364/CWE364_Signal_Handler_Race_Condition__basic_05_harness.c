```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// Declare prototypes for the Juliet entry functions
void CWE364_Signal_Handler_Race_Condition__basic_05_bad(void);
void CWE364_Signal_Handler_Race_Condition__basic_05_good(void);

// Function to run the BAD case
void run_bad(void) {
    printf("Running bad case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_05_bad();
    printf("Finished running bad case.\n");
}

// Function to run the GOOD case
void run_good(void) {
    printf("Running good case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_05_good();
    printf("Finished running good case.\n");
}

// Main function to control execution
int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```