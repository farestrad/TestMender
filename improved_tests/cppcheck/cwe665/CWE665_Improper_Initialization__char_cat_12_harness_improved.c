```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CWE665_Improper_Initialization__char_cat_12_bad(void);
void CWE665_Improper_Initialization__char_cat_12_good(void);

/*
 * Helper to force both branches in the Juliet sources to execute the "bad" or "good" code unconditionally.
 * cppcheck and Juliet both use globalReturnsTrueOrFalse() for control flow.
 */
int globalReturnsTrueOrFalse(void) { return 1; } // always take the "bad" branch in the BAD run
int globalReturnsTrue(void) { return 1; }
int globalReturnsFalse(void) { return 0; }

#ifndef OMITBAD
/* BAD: Attempt to maximize the chance of overflow or use of uninitialized buffer.
   Juliet bad() allocates char data[100] but does NOT initialize it, so strncat might
   read past the initial '\0' when scanning the string length.
   To strengthen: Fill stack below data with nonzero bytes, and use a very large source for strcat.
*/

__attribute__((noinline))
void poison_stack_below(char *data, size_t len) {
    volatile char *p = (volatile char*)data - 128;
    for (int i = 0; i < 128; ++i) {
        p[i] = 'X';
    }
}

void run_bad(void) {
    printf("Running bad case...\n");
    /* Replicate how Juliet BAD uses a local buffer, then poison stack below it to encourage overflow. */
    char dummy[128];
    memset(dummy, 'A', sizeof(dummy)); // fill unused stack with 'A'
    poison_stack_below(dummy + 128, 128);

    CWE665_Improper_Initialization__char_cat_12_bad();

    printf("Finished bad case.\n");
}
#endif

#ifndef OMITGOOD
void run_good(void) {
    printf("Running good case...\n");
    CWE665_Improper_Initialization__char_cat_12_good();
    printf("Finished good case.\n");
}
#endif

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```