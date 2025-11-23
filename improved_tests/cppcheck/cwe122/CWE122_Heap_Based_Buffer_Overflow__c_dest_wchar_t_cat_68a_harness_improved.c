```c
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

wchar_t * CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_badData;
wchar_t * CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_goodG2BData;

void CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_good(void);

#define SRC_STRING L"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"  /* 50 'A's + NUL */

/* BAD setup: Allocates insufficient buffer for concatenation and sets global */
void run_bad(void) {
    size_t dataSize = 50 * sizeof(wchar_t); /* intentionally under-allocate */
    CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_badData = (wchar_t *)malloc(dataSize);
    if (CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_badData == NULL) { exit(1); }
    /* Initialize as empty string */
    CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_badData[0] = L'\0';
    CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_bad();
    free(CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_badData);
}

/* GOOD setup: Allocates sufficient buffer for concatenation and sets global */
void run_good(void) {
    size_t dataSize = 100 * sizeof(wchar_t); /* large enough for concatenation */
    CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_goodG2BData = (wchar_t *)malloc(dataSize);
    if (CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_goodG2BData == NULL) { exit(1); }
    /* Initialize as empty string */
    CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_goodG2BData[0] = L'\0';
    CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_good();
    free(CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_goodG2BData);
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