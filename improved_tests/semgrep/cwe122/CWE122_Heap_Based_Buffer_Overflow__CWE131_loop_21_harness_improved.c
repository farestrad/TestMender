```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Juliet function prototypes
void CWE122_Heap_Based_Buffer_Overflow__CWE131_loop_21_badSink(int * data);
void CWE122_Heap_Based_Buffer_Overflow__CWE131_loop_21_goodG2BSink(int * data);

// Helper: BAD path - allocate buffer with insufficient size, fill values, and call BAD sink
void run_bad(void) {
    printf("Running BAD case...\n");
    // Intentionally allocate a too-small buffer (e.g., 5 ints)
    size_t under_size = 5 * sizeof(int);
    int * data = (int *)malloc(under_size);
    if (data == NULL) { exit(1); }
    // Fill the buffer with known values
    for (size_t i = 0; i < 5; i++) {
        data[i] = (int)i;
    }
    // BAD sink expects 10 ints and uses a loop of 10
    CWE122_Heap_Based_Buffer_Overflow__CWE131_loop_21_badSink(data);
    free(data);
}

// Helper: GOOD path - allocate a sufficiently large buffer
void run_good(void) {
    printf("Running GOOD case...\n");
    // Correct allocation: buffer for 10 ints
    size_t good_size = 10 * sizeof(int);
    int * data = (int *)malloc(good_size);
    if (data == NULL) { exit(1); }
    // Fill with values
    for (size_t i = 0; i < 10; i++) {
        data[i] = (int)i;
    }
    CWE122_Heap_Based_Buffer_Overflow__CWE131_loop_21_goodG2BSink(data);
    free(data);
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