```c
#include <stdio.h>
#include <stdlib.h>

void CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B1(void);
void CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B2(void);

void run_bad(void) {
    printf("Running bad case...\n");
    CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_bad();
}

void run_good(void) {
    printf("Running good case 1...\n");
    CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B1();
    
    printf("Running good case 2...\n");
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