```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

/* Declare prototypes for the Juliet entry functions */
void CWE122_Heap_Based_Buffer_Overflow__CWE135_07_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__CWE135_07_good(void);

/* Function to drive the BAD path */
void run_bad(void) {
    printf("Running BAD test case...\n");
    CWE122_Heap_Based_Buffer_Overflow__CWE135_07_bad();
}

/* Function to drive the GOOD path */
void run_good(void) {
    printf("Running GOOD test case...\n");
    CWE122_Heap_Based_Buffer_Overflow__CWE135_07_good();
}

/* Main function to control test execution */
int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```