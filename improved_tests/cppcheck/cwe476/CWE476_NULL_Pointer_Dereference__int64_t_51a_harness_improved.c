```c
#include <stdio.h>
#include <stdint.h>

// Function prototypes for the Juliet entry functions
void CWE476_NULL_Pointer_Dereference__int64_t_51_bad(void);
void CWE476_NULL_Pointer_Dereference__int64_t_51_good(void);

// Run the BAD case
void run_bad(void) {
    CWE476_NULL_Pointer_Dereference__int64_t_51_bad();
}

// Run the GOOD case
void run_good(void) {
    CWE476_NULL_Pointer_Dereference__int64_t_51_good();
}

// Main function to control flow
int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```