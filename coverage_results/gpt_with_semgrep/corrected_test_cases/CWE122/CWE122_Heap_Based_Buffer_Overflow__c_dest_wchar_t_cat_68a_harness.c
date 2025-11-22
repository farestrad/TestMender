// ```c
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

wchar_t * CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_badData;
wchar_t * CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_goodG2BData;

void CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68b_badSink();
void CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68b_goodG2BSink();

void run_bad(void) {
    wchar_t * data;
    data = NULL;
    data = (wchar_t *)malloc(50 * sizeof(wchar_t));
    if (data == NULL) { exit(-1); }
    data[0] = L'\0'; /* null terminate */
    CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_badData = data;
    
    printf("Running bad function...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68b_badSink();
}

void run_good(void) {
    wchar_t * data;
    data = NULL;
    data = (wchar_t *)malloc(100 * sizeof(wchar_t));
    if (data == NULL) { exit(-1); }
    data[0] = L'\0'; /* null terminate */
    CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_goodG2BData = data;
    
    printf("Running good function...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68b_goodG2BSink();
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