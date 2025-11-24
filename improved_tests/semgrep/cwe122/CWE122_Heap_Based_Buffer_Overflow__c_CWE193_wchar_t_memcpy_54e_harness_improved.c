```c
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_wchar_t_memcpy_54e_badSink(wchar_t * data);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_wchar_t_memcpy_54e_goodG2BSink(wchar_t * data);

#define SRC_STRING L"AAAAAAAAAA" /* 10 characters */

void run_bad(void) {
    wchar_t *data;
    /* FLAW: Allocate memory for 10 wchar_t, no room for NULL terminator */
    data = (wchar_t *)malloc(10 * sizeof(wchar_t));
    if (data == NULL) {
        exit(EXIT_FAILURE);
    }
    /* Initialize data with a valid string of 10 non-null wchar_t -- NO null terminator */
    wmemcpy(data, SRC_STRING, 10);
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_wchar_t_memcpy_54e_badSink(data);
    free(data);
}

void run_good(void) {
    wchar_t *data;
    /* FIX: Allocate space for 10 chars + the NULL terminator */
    data = (wchar_t *)malloc((10 + 1) * sizeof(wchar_t));
    if (data == NULL) {
        exit(EXIT_FAILURE);
    }
    /* Initialize data with string plus null terminator */
    wmemcpy(data, SRC_STRING, 10);
    data[10] = L'\0';
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_wchar_t_memcpy_54e_goodG2BSink(data);
    free(data);
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