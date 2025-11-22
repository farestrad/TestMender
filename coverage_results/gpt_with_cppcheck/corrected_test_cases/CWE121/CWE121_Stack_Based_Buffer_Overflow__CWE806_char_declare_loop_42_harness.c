// ```c
#include <stdio.h>
#include <string.h>

void CWE121_Stack_Based_Buffer_Overflow__CWE806_char_declare_loop_42_bad(void);
void CWE121_Stack_Based_Buffer_Overflow__CWE806_char_declare_loop_42_good(void);

void run_bad(void) {
    printf("Running bad test case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE806_char_declare_loop_42_bad();
    printf("Finished bad test case.\n");
}

void run_good(void) {
    printf("Running good test case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE806_char_declare_loop_42_good();
    printf("Finished good test case.\n");
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