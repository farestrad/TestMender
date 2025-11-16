```c
#include <stdio.h>
#include <stdlib.h>

// Prototypes for the Juliet good and bad functions
void CWE364_Signal_Handler_Race_Condition__basic_16_bad(void);
void CWE364_Signal_Handler_Race_Condition__basic_16_good(void);

void run_bad(void) {
    printf("Running bad()...\n");
    CWE364_Signal_Handler_Race_Condition__basic_16_bad();
    printf("Finished running bad()\n");
}

void run_good(void) {
    printf("Running good()...\n");
    CWE364_Signal_Handler_Race_Condition__basic_16_good();
    printf("Finished running good()\n");
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