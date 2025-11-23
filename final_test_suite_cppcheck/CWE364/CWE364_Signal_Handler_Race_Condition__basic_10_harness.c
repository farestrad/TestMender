```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// Prototypes for the Juliet entry functions
#ifndef OMITBAD
void CWE364_Signal_Handler_Race_Condition__basic_10_bad(void);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE364_Signal_Handler_Race_Condition__basic_10_good(void);
#endif /* OMITGOOD */

// Function to test the bad case
void run_bad(void) {
    printf("Running bad case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_10_bad();
    printf("Finished bad case.\n");
}

// Function to test the good case
void run_good(void) {
    printf("Running good case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_10_good();
    printf("Finished good case.\n");
}

// Main function to control the flow based on the mode
int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```