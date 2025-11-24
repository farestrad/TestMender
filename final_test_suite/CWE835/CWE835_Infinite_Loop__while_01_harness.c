```c
#include <stdio.h>

void CWE835_Infinite_Loop__while_01_bad(void);
void CWE835_Infinite_Loop__while_01_good(void);

void run_bad(void) {
    printf("Running BAD version...\n");
    CWE835_Infinite_Loop__while_01_bad();
    printf("Completed BAD version.\n");
}

void run_good(void) {
    printf("Running GOOD version...\n");
    CWE835_Infinite_Loop__while_01_good();
    printf("Completed GOOD version.\n");
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