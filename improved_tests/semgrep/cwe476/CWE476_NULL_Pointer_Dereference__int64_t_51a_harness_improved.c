```c
#include <stdio.h>
#include <stdint.h>

// Prototypes for the Juliet entry functions
void CWE476_NULL_Pointer_Dereference__int64_t_51_bad(void);
void CWE476_NULL_Pointer_Dereference__int64_t_51_good(void);

// Function to run the BAD test case
void run_bad(void) {
    printf("Running BAD test case...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_51_bad();
    printf("Finished BAD test case.\n");
}

// Function to run the GOOD test case
void run_good(void) {
    printf("Running GOOD test case...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_51_good();
    printf("Finished GOOD test case.\n");
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