// ```c
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

wchar_t * CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_badData;
wchar_t * CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_goodG2BData;

void CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_good(void);

void run_bad(void) {
    printf("Running bad function...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_bad();
}

void run_good(void) {
    printf("Running good function...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_good();
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