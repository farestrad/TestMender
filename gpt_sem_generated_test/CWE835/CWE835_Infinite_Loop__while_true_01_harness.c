```c
#include "std_testcase.h"

#ifndef OMITBAD
void CWE835_Infinite_Loop__while_true_01_bad(void);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE835_Infinite_Loop__while_true_01_good(void);
#endif /* OMITGOOD */

void run_bad(void) {
    printf("Executing BAD function: CWE835_Infinite_Loop__while_true_01_bad\n");
    CWE835_Infinite_Loop__while_true_01_bad();
    printf("Finished BAD function execution\n");
}

void run_good(void) {
    printf("Executing GOOD function: CWE835_Infinite_Loop__while_true_01_good\n");
    CWE835_Infinite_Loop__while_true_01_good();
    printf("Finished GOOD function execution\n");
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