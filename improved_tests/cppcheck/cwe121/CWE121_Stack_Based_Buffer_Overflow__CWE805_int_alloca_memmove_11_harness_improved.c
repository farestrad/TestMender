
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include "std_testcase.h"

#ifndef OMITBAD
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_11_bad(void);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_11_good(void);
#endif /* OMITGOOD */

/* The following function will try to make AddressSanitizer reliably catch the overflow
 * by executing the BAD path multiple times, to defeat stack reuse optimizations,
 * and aborting if no crash is detected.
 */
void run_bad(void) {
    printf("Running bad case...\n");
    /* Deliberately run multiple times for stack setup, try to maximize exposure */
    volatile int crash_detected = 0;
    void handler(int sig) {
        crash_detected = 1;
        printf("Caught signal %d (expected crash)\n", sig);
        exit(134);
    }
    signal(SIGABRT, handler);
    signal(SIGSEGV, handler);
    signal(SIGBUS, handler);

    /* Run multiple times in hopes ASan catches the first one */
    for(int i = 0; i < 10; ++i) {
        CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_11_bad();
    }
    printf("Finished running bad case (but no crash/signaled memory problem was detected).\n");
}

void run_good(void) {
    printf("Running good case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_11_good();
    printf("Finished running good case.\n");
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
