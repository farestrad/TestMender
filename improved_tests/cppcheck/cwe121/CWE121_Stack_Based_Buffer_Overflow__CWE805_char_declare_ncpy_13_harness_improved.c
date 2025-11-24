
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncpy_13_bad(void);
void CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncpy_13_good(void);

static void fill_large_input(void)
{
    /* Create a very large environment variable or stdin input if the Juliet file uses them */
    /* If the Juliet code reads from stdin or getenv, we set up an oversized buffer. */
    /* But, for most CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncpy_13_* cases,
       the overflow comes from using a local buffer that's too small, and a source buffer 
       that's much bigger. To help, try to ensure the source buffer (if used) is very large. */
    /* If needed, this could be extended to set argv or such. */
}

/* 
 * The Juliet testcases for CWE805 and ncpy patterns typically allocate two stack
 * buffers (small and large) and choose which one to use based on a global constant.
 * The copy length is usually 100, matching the large buffer, but the small is often 50.
 *
 * We want to set up the test so that:
 *   - GOOD: no crash, no ASan error
 *   - BAD: stack buffer overflow detected by ASan
 *
 * To maximize the probability, call the function as normal (they're parameterless),
 * but make sure the stack is "busy" so ASan reliably detects an overwrite.
 */

static void poison_stack(void)
{
    /* Force use of more stack frames to make ASan detection more reliable. */
    char junk[128];
    memset(junk, 'A', sizeof(junk));
    /* Use junk so it is not optimized out */
    volatile char sink = junk[0];
    (void)sink;
}

void run_bad(void) {
    printf("Running bad test case...\n");
    poison_stack();
    CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncpy_13_bad();
}

void run_good(void) {
    printf("Running good test case...\n");
    poison_stack();
    CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncpy_13_good();
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}