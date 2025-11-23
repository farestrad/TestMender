```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// Function prototypes for Juliet entry functions
void CWE364_Signal_Handler_Race_Condition__basic_14_bad(void);
void CWE364_Signal_Handler_Race_Condition__basic_14_good(void);

// Function to run the BAD case
void run_bad(void) {
    printf("Running BAD case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_14_bad();
    printf("Finished BAD case.\n");
}

// Function to run the GOOD case
void run_good(void) {
    printf("Running GOOD case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_14_good();
    printf("Finished GOOD case.\n");
}

// Main function to direct execution based on compilation mode
int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```