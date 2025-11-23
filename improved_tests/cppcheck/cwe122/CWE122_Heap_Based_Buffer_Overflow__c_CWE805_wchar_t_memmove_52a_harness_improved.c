```c
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

/* Juliet function prototypes */
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52_good(void);

int main(void)
{
#ifdef TEST_MODE_BAD
    printf("Running bad case...\n");
    fflush(stdout);
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52_bad();
    printf("Finished bad case.\n");
    fflush(stdout);
#else
    printf("Running good case...\n");
    fflush(stdout);
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52_good();
    printf("Finished good case.\n");
    fflush(stdout);
#endif
    return 0;
}
```