```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prototype declarations for the Juliet entry points
void CWE122_Heap_Based_Buffer_Overflow__CWE131_loop_21_bad_sink(int * data);
void CWE122_Heap_Based_Buffer_Overflow__CWE131_loop_21_goodG2B_sink(int * data);

// The BAD and GOOD sink functions operate on a buffer of ints,
// Typically the BAD version allocates insufficient space, we want to trigger this.

// Function to run the bad test case -- allocate minimal buffer to provoke overflow
void run_bad(void) {
    int *data = NULL;
    size_t small_size = 5;   // deliberately small
    size_t copy_size = 10;   // Juliet often copies 10 ints

    printf("Running bad test case...\n");

    // Allocate small buffer to provoke overflow
    data = (int *)malloc(small_size * sizeof(int));
    if (!data) {
        printf("malloc failed\n");
        exit(1);
    }
    // Initialize buffer so access after small_size is likely to trip ASan
    memset(data, 0xA5, small_size * sizeof(int));

    CWE122_Heap_Based_Buffer_Overflow__CWE131_loop_21_bad_sink(data);

    // If the bug doesn't crash, we still need to permit use-after-free detection etc.
    free(data);

    printf("Finished bad test case.\n");
}

// Function to run the good test case -- allocate correct buffer
void run_good(void) {
    int *data = NULL;
    size_t good_size = 10;

    printf("Running good test case...\n");

    // Allocate sufficient space
    data = (int *)malloc(good_size * sizeof(int));
    if (!data) {
        printf("malloc failed\n");
        exit(1);
    }
    memset(data, 0xA5, good_size * sizeof(int));

    CWE122_Heap_Based_Buffer_Overflow__CWE131_loop_21_goodG2B_sink(data);

    free(data);

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