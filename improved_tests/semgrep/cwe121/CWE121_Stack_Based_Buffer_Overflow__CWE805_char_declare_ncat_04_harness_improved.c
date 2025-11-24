```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Juliet entry points */
void CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncat_04_bad(void);
void CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncat_04_good(void);

/* 
 * Many Juliet stack-based buffer overflow testcases read from stdin or have fixed input,
 * but some accept direct input by global variables or rely on the runtime environment.
 * For SEMGREP-guided harnessing, we strengthen the overflow: in these Juliet ncat cases,
 * the sink is usually strncat, potentially using src that could overflow dest if we supply a large enough buffer.
 * 
 * Some testcases use static variables or global control to set the overflow size.
 * We use a large enough input that will overflow any local destination buffer of 50 bytes if copied.
 * However, most "bad" Juliet ncat cases are fully deterministic and do not accept external input.
 * Thus, we simply invoke the entry points separately and let the BAD path be triggered.
 * 
 * To ensure the overflow occurs, we can call the bad function in a child process so 
 * the AddressSanitizer error does not terminate the whole test suite if managed externally 
 * (useful for some test runners), but that's optional.
 */

int main(void) {
#ifdef TEST_MODE_BAD
    printf("Executing bad test case:\n");
    /*
     * Strengthen the bug manifestation by calling bad function multiple times in succession.
     * The bad path in Juliet will overflow a small stack buffer (usually size 50) by copying or concatenating
     * 100 bytes to it using strncat or related. No extra data preparation needed.
     * So, directly invoke the function; the vulnerability is deterministic if compiled with -DTEST_MODE_BAD.
     */
    CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncat_04_bad();

    /*
     * Optionally, force stack usage to help stack protector/ASan exposures (rarely needed):
     * volatile char fill[100] = {0}; (unused in this case)
     */
#else
    printf("Executing good test case:\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncat_04_good();
#endif
    return 0;
}
```