```c
#include <stdio.h>

/* Function declarations from Juliet CWE121 test case */
void CWE121_Stack_Based_Buffer_Overflow__src_wchar_t_declare_cat_54c_bad(void);
void CWE121_Stack_Based_Buffer_Overflow__src_wchar_t_declare_cat_54c_goodG2B(void);

int main(void) {
#ifdef TEST_MODE_BAD
    /* BAD: Run the Juliet bad function. It will set up the overflow itself. */
    printf("Running bad test...\n");
    CWE121_Stack_Based_Buffer_Overflow__src_wchar_t_declare_cat_54c_bad();
#else
    /* GOOD: Run the Juliet good function. It will set up the safe case itself. */
    printf("Running good test...\n");
    CWE121_Stack_Based_Buffer_Overflow__src_wchar_t_declare_cat_54c_goodG2B();
#endif
    return 0;
}
```