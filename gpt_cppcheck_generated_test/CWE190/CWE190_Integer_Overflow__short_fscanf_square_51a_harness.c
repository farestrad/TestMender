```c
#include <stdio.h>

/* Function declarations for the Juliet entry functions */
void CWE190_Integer_Overflow__short_fscanf_square_51_bad(void);
void CWE190_Integer_Overflow__short_fscanf_square_51_good(void);

/* Function to run the bad case */
void run_bad(void) {
    printf("Running bad case...\n");
    CWE190_Integer_Overflow__short_fscanf_square_51_bad();
}

/* Function to run the good case */
void run_good(void) {
    printf("Running good case...\n");
    CWE190_Integer_Overflow__short_fscanf_square_51_good();
}

/* Main function */
int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```