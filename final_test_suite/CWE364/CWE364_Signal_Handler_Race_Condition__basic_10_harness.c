```c
#include <stdio.h>
#include <stdlib.h>

void CWE364_Signal_Handler_Race_Condition__basic_10_bad(void);
void CWE364_Signal_Handler_Race_Condition__basic_10_good(void);

void run_bad(void) {
    printf("Running bad function...\n");
    CWE364_Signal_Handler_Race_Condition__basic_10_bad();
}

void run_good(void) {
    printf("Running good function...\n");
    CWE364_Signal_Handler_Race_Condition__basic_10_good();
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