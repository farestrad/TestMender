```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// Prototypes for the Juliet entry functions
void CWE364_Signal_Handler_Race_Condition__basic_05_bad(void);
void CWE364_Signal_Handler_Race_Condition__basic_05_good(void);

// Run the bad case
void run_bad(void) {
    printf("Running bad case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_05_bad();
    printf("Finished bad case.\n");
}

// Run the good case
void run_good(void) {
    printf("Running good case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_05_good();
    printf("Finished good case.\n");
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
```