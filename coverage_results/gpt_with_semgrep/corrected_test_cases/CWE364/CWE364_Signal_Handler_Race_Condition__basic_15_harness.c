// ```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

typedef struct _structSigAtomic
{
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_15StructSigAtomicBad = NULL;
structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_15StructSigAtomicGood = NULL;

static void helperBad(int sig)
{
    if (CWE364_Signal_Handler_Race_Condition__basic_15StructSigAtomicBad != NULL)
    {
        CWE364_Signal_Handler_Race_Condition__basic_15StructSigAtomicBad->val = 2;
    }
}

static void helperGood(int sig)
{
    if (CWE364_Signal_Handler_Race_Condition__basic_15StructSigAtomicGood != NULL)
    {
        CWE364_Signal_Handler_Race_Condition__basic_15StructSigAtomicGood->val = 2;
    }
}

#ifndef OMITBAD
void CWE364_Signal_Handler_Race_Condition__basic_15_bad(void);
#endif /* OMITBAD */
#ifndef OMITGOOD
void CWE364_Signal_Handler_Race_Condition__basic_15_good(void);
#endif /* OMITGOOD */

void run_bad(void)
{
    printf("Running BAD test case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_15_bad();
}

void run_good(void)
{
    printf("Running GOOD test case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_15_good();
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