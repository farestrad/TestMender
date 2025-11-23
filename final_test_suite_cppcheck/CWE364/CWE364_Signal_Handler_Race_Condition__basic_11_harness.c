```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// Prototypes for the original Juliet entry functions
void CWE364_Signal_Handler_Race_Condition__basic_11_bad(void);
void CWE364_Signal_Handler_Race_Condition__basic_11_good(void);

void run_bad(void) {
    printf("Running bad test case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_11_bad();
    printf("Completed bad test case.\n");
}

void run_good(void) {
    printf("Running good test case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_11_good();
    printf("Completed good test case.\n");
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