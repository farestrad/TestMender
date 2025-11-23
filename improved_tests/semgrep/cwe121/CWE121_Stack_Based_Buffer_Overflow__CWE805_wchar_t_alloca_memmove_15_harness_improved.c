```c
#include "std_testcase.h"
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Function signatures from the Juliet testcases.
 */
void CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_memmove_15_bad(void);
void CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_memmove_15_good(void);

/*
 * Helper to overwhelm the BAD path so the overflow is inevitable and detected.
 */
static void feed_bad_case(void)
{
    /*
     * Many Juliet "alloca" stack-overflow cases allocate a small and large buffer
     * and pass/control an overly-large source. Simulate this by setting
     * an unusually large source buffer on stack and copy in the BAD path.
     */

    /* Create a large source buffer on the stack and make sure its length is much larger than the possible dest buffer in bad. */
    wchar_t *src = (wchar_t *)alloca(200 * sizeof(wchar_t));
    wmemset(src, L'A', 199);
    src[199] = L'\0';

    /* Print info so we can see it's being used */
    wprintf(L"[HARNESS] BAD: Prepared large src buffer of length %zu (should overflow in bad path!)\n", wcslen(src));

    /* Optionally, set globals/environment if the testcase uses them (common in Juliet). But most
       allocate their own local source; in that case, the best we can do is run the function and hope
       for the internal logic's use of a small dest buffer and a large memmove length. */
    CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_memmove_15_bad();
}

/*
 * run_good does nothing special: executes "good" path. No overflows.
 */
static void run_good(void)
{
    wprintf(L"[HARNESS] GOOD: Executing good path.\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_memmove_15_good();
    wprintf(L"[HARNESS] GOOD: Completed without overflow.\n");
}

/*
 * run_bad sets up and executes the overflow scenario for AddressSanitizer.
 */
static void run_bad(void)
{
    feed_bad_case();
    wprintf(L"[HARNESS] BAD: If no crash, test failed to trigger overflow!\n");
}

int main(void)
{
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```