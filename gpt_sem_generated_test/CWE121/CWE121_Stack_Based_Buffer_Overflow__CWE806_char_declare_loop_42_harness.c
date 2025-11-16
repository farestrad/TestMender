```c
#include "std_testcase.h"

void CWE121_Stack_Based_Buffer_Overflow__CWE806_char_declare_loop_42_bad(void);
void CWE121_Stack_Based_Buffer_Overflow__CWE806_char_declare_loop_42_good(void);

void run_bad(void) {
    printf("Running bad case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE806_char_declare_loop_42_bad();
    printf("Finished bad case.\n");
}

void run_good(void) {
    printf("Running good case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE806_char_declare_loop_42_good();
    printf("Finished good case.\n");
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