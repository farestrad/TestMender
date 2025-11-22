```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// Prototypes for the Juliet entry functions
void CWE364_Signal_Handler_Race_Condition__basic_04_bad(void);
void CWE364_Signal_Handler_Race_Condition__basic_04_good(void);

void run_bad(void) {
    printf("Running bad test...\n");
    CWE364_Signal_Handler_Race_Condition__basic_04_bad();
    printf("Finished running bad test.\n");
}

void run_good(void) {
    printf("Running good test...\n");
    CWE364_Signal_Handler_Race_Condition__basic_04_good();
    printf("Finished running good test.\n");
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