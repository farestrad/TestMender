// ```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void CWE364_Signal_Handler_Race_Condition__basic_07_bad(void);
void CWE364_Signal_Handler_Race_Condition__basic_07_good(void);

void run_bad(void) {
    printf("Running bad test case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_07_bad();
}

void run_good(void) {
    printf("Running good test case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_07_good();
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
// ```