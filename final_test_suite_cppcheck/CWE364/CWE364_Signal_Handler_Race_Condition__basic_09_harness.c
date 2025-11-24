```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// Prototypes for the Juliet entry functions
void CWE364_Signal_Handler_Race_Condition__basic_09_bad(void);
void CWE364_Signal_Handler_Race_Condition__basic_09_good(void);

void run_bad(void) {
    printf("Running bad case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_09_bad();
    printf("Finished running bad case.\n");
}

void run_good(void) {
    printf("Running good case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_09_good();
    printf("Finished running good case.\n");
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```