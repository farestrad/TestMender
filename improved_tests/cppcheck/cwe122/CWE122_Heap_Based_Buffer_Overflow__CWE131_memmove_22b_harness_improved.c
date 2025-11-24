```c
#include <stdio.h>
#include <stdlib.h>

/* Extern the Juliet global control variables */
extern int CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_badGlobal;
extern int CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B1Global;
extern int CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B2Global;

/* Extern the Juliet entry points */
void CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B1(void);
void CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B2(void);

void run_bad(void) {
    /*
     * The BAD path: Set the global flag to ensure the bad source is selected,
     * which omits space for the NULL terminator and will cause
     * heap buffer overflow in the sink when memmove() is called.
     */
    CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_badGlobal = 1;
    CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_bad();
}

void run_good(void) {
    /*
     * GOOD G2B1: Set the flag so the good source (with proper allocation) is used.
     * G2B1 as implemented in the Juliet template requires the flag to be 0 for the good path.
     */
    CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B1Global = 0;
    CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B1();

    /*
     * GOOD G2B2: Set the flag so the good source is used directly.
     * G2B2 as implemented in the Juliet template requires the flag to be 1 for the good path.
     */
    CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B2Global = 1;
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