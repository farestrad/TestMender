```c
#include <stdio.h>
#include <string.h>

void CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncat_04_bad(void);
void CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncat_04_good(void);

void run_bad(void) {
    printf("Running BAD test case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncat_04_bad();
}

void run_good(void) {
    printf("Running GOOD test case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncat_04_good();
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