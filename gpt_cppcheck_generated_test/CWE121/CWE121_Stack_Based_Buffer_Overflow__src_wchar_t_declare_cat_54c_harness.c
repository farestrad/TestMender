```c
#include <stdio.h>
#include <wchar.h>

void CWE121_Stack_Based_Buffer_Overflow__src_wchar_t_declare_cat_54c_bad(void);
void CWE121_Stack_Based_Buffer_Overflow__src_wchar_t_declare_cat_54c_goodG2B(void);

void run_bad(void) {
    wchar_t data[100];
    // Initialize data with a large string to trigger the overflow vulnerability
    wmemset(data, L'A', 99); // Fill with 'A's, make sure it's large
    data[99] = L'\0'; // Null terminate

    // Call the bad function
    printf("Running bad test...\n");
    CWE121_Stack_Based_Buffer_Overflow__src_wchar_t_declare_cat_54c_bad();
}

void run_good(void) {
    wchar_t data[50];
    // Initialize data with a small string for safe execution
    wmemset(data, L'A', 49); // Fill with 'A's but smaller
    data[49] = L'\0'; // Null terminate

    // Call the good function
    printf("Running good test...\n");
    CWE121_Stack_Based_Buffer_Overflow__src_wchar_t_declare_cat_54c_goodG2B();
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