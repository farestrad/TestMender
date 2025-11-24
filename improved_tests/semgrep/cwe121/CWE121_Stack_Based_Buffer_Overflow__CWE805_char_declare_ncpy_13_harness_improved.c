```c
#include "std_testcase.h"
#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef OMITBAD
void CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncpy_13_bad();
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncpy_13_good();
#endif /* OMITGOOD */

/* 
 * For the CWE121 Juliet family, the sink copies a large string (e.g. 100 chars) to a possibly small stack buffer.
 * To ensure the BAD path triggers an AddressSanitizer error, we provoke a large copy with a small buffer.
 * We do this by setting a large source buffer on the stack and using it as the global input buffer if possible,
 * or by making sure the globals used by BAD/GOOD are filled before the call.
 * However, since Juliet stacks its own buffers, our main influence is to prepare the environment and 
 * call the entrypoints only.
 */

void run_bad(void)
{
    printf("Running bad case...\n");
    /* The CWE121 BAD function internally sets up a small dest buffer and copies 100 chars into it.
       There is no externally visible buffer to manipulate, but in some Juliet cases manipulating stdin or
       argv could matter. For this case, just call the bad function. */
    CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncpy_13_bad();

    /* Add an immediate operation to touch stack post-call to encourage detection of clobbered stack. */
    volatile char touchstack[8];
    memset((void*)touchstack, 0xA5, sizeof(touchstack));

    printf("Finished running bad case.\n");
}

void run_good(void)
{
    printf("Running good case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_char_declare_ncpy_13_good();
    printf("Finished running good case.\n");
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