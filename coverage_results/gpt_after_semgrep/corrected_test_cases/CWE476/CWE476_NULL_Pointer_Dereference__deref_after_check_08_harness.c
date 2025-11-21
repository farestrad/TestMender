// ```c
#include <stdio.h>

// Prototypes for the Juliet entry functions
#ifndef OMITBAD
void CWE476_NULL_Pointer_Dereference__deref_after_check_08_bad(void);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE476_NULL_Pointer_Dereference__deref_after_check_08_good(void);
#endif /* OMITGOOD */

void run_bad(void) {
    printf("Running bad() function...\n");
    CWE476_NULL_Pointer_Dereference__deref_after_check_08_bad();
    printf("Finished running bad() function.\n");
}

void run_good(void) {
    printf("Running good() function...\n");
    CWE476_NULL_Pointer_Dereference__deref_after_check_08_good();
    printf("Finished running good() function.\n");
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
// ```