```c
#include <stdio.h>
#include <stdlib.h>

// Prototype declarations for the Juliet entry points
void CWE122_Heap_Based_Buffer_Overflow__CWE131_loop_21_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__CWE131_loop_21_good(void);

// Function to run the bad test case
void run_bad(void) {
    printf("Running bad test case...\n");
    CWE122_Heap_Based_Buffer_Overflow__CWE131_loop_21_bad();
    printf("Finished bad test case.\n");
}

// Function to run the good test cases
void run_good(void) {
    printf("Running good test case...\n");
    CWE122_Heap_Based_Buffer_Overflow__CWE131_loop_21_good();
    printf("Finished good test case.\n");
}

// Main function to switch between bad and good test cases
int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```