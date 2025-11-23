```c
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

/* Function prototypes for the Juliet entry functions */
void CWE476_NULL_Pointer_Dereference__int64_t_54_bad(void);
void CWE476_NULL_Pointer_Dereference__int64_t_54_good(void);

int main(void) {
    /*
     * Juliet GOOD and BAD entrypoints for CWE476 54 flow variants already
     * internally set their data pointer correctly (NULL for bad, non-NULL for good).
     * We invoke and check for abnormal termination.
     */
#ifdef TEST_MODE_BAD
    printf("Running bad case...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_54_bad();
    printf("Finished bad case (unexpected: should have crashed or ASan).\n");
#else
    printf("Running good case...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_54_good();
    printf("Finished good case.\n");
#endif
    return 0;
}
```