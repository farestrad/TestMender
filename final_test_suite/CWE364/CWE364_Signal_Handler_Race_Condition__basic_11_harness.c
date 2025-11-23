```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// Prototypes for BAD and GOOD functions from the Juliet test case
void CWE364_Signal_Handler_Race_Condition__basic_11_bad(void);
void CWE364_Signal_Handler_Race_Condition__basic_11_good(void);

// Function to test BAD case
void run_bad(void) {
    printf("Running BAD case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_11_bad();
    printf("Finished BAD case.\n");
}

// Function to test GOOD case
void run_good(void) {
    printf("Running GOOD case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_11_good();
    printf("Finished GOOD case.\n");
}

// Main function to choose between GOOD and BAD cases
int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```