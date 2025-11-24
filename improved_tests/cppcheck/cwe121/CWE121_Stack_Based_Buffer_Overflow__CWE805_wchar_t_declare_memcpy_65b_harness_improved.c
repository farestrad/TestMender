```c
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

void CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_declare_memcpy_65b_badSink(wchar_t * data);
void CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_declare_memcpy_65b_goodG2BSink(wchar_t * data);

void run_bad(void) {
    wchar_t *data;
    /* FLAW: Set data pointer to a small buffer */
    wchar_t badBuffer[50];
    data = badBuffer;
    /* Initialize buffer to make sure it's not an uninitialized stack */
    wmemset(data, L'A', 49);
    data[49] = L'\0';
    printf("Running BAD test case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_declare_memcpy_65b_badSink(data);
}

void run_good(void) {
    wchar_t *data;
    /* FIX: Set data pointer to a large buffer */
    wchar_t goodBuffer[100];
    data = goodBuffer;
    /* Initialize buffer as in the 'bad', but large enough */
    wmemset(data, L'A', 99);
    data[99] = L'\0';
    printf("Running GOOD test case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_declare_memcpy_65b_goodG2BSink(data);
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