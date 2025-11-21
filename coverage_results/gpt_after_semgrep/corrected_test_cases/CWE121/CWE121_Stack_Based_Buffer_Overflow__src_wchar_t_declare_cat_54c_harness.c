// ```c
#include <stdio.h>
#include <wchar.h>

void CWE121_Stack_Based_Buffer_Overflow__src_wchar_t_declare_cat_54c_badSink(wchar_t * data);
void CWE121_Stack_Based_Buffer_Overflow__src_wchar_t_declare_cat_54c_goodG2BSink(wchar_t * data);

void run_bad(void) {
    wchar_t data[50];
    // Initialize data as a large string to trigger the overflow
    wmemset(data, L'A', 100);
    data[99] = L'\0'; // Null terminate
    
    wprintf(L"Running bad case with overflow string: %ls\n", data);
    CWE121_Stack_Based_Buffer_Overflow__src_wchar_t_declare_cat_54c_badSink(data);
}

void run_good(void) {
    wchar_t data[50];
    // Initialize data as a small string
    wmemset(data, L'A', 49);
    data[49] = L'\0'; // Null terminate
    
    wprintf(L"Running good case with safe string: %ls\n", data);
    CWE121_Stack_Based_Buffer_Overflow__src_wchar_t_declare_cat_54c_goodG2BSink(data);
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