```c
#include <stdio.h>
#include <limits.h>

// Declare prototypes for the Juliet entry functions
void CWE190_Integer_Overflow__int_max_add_67_bad(void);
void CWE190_Integer_Overflow__int_max_add_67_good(void);

// Function to run the bad case
void run_bad(void) {
    printf("Running bad case...\n");
    CWE190_Integer_Overflow__int_max_add_67_bad();
    printf("Finished bad case.\n");
}

// Function to run the good case
void run_good(void) {
    printf("Running good case...\n");
    CWE190_Integer_Overflow__int_max_add_67_good();
    printf("Finished good case.\n");
}

// Main function to control execution flow
int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```