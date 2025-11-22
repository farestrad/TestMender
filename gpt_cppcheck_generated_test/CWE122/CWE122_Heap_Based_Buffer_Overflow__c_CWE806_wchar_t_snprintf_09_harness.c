```c
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

void CWE122_Heap_Based_Buffer_Overflow__c_CWE806_wchar_t_snprintf_09_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE806_wchar_t_snprintf_09_good(void);

void run_bad(void) {
    printf("Running bad case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE806_wchar_t_snprintf_09_bad();
}

void run_good(void) {
    printf("Running good case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE806_wchar_t_snprintf_09_good();
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