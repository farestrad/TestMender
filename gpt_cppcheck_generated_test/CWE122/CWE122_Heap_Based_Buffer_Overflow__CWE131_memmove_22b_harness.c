```c
#include <stdio.h>
#include <stdlib.h>

extern int CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_badGlobal;
extern int CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B1Global;
extern int CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B2Global;

void CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B1(void);
void CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B2(void);

void run_bad(void) {
    printf("Running BAD test case...\n");
    CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_badGlobal = 1; // Trigger the vulnerability
    CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_bad();
}

void run_good(void) {
    printf("Running GOOD test case G2B1...\n");
    CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B1Global = 0; // Use safe allocation and bad sink
    CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B1();
    
    printf("Running GOOD test case G2B2...\n");
    CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B2Global = 1; // Use safe allocation directly
    CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B2();
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