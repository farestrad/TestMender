// ```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

typedef struct _structSigAtomic
{
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_08StructSigAtomicBad = NULL;
structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_08StructSigAtomicGood = NULL;

static void helperBad(int sig)
{
    if (CWE364_Signal_Handler_Race_Condition__basic_08StructSigAtomicBad != NULL)
    {
        CWE364_Signal_Handler_Race_Condition__basic_08StructSigAtomicBad->val = 2;
    }
}

static void helperGood(int sig)
{
    if (CWE364_Signal_Handler_Race_Condition__basic_08StructSigAtomicGood != NULL)
    {
        CWE364_Signal_Handler_Race_Condition__basic_08StructSigAtomicGood->val = 2;
    }
}

static int staticReturnsTrue()
{
    return 1;
}

static int staticReturnsFalse()
{
    return 0;
}

#ifndef OMITBAD

void CWE364_Signal_Handler_Race_Condition__basic_08_bad(void);

#endif /* OMITBAD */

#ifndef OMITGOOD

void CWE364_Signal_Handler_Race_Condition__basic_08_good(void);

#endif /* OMITGOOD */

void run_bad(void) {
    printf("Running bad function...\n");
    CWE364_Signal_Handler_Race_Condition__basic_08_bad();
    printf("Finished running bad function.\n");
}

void run_good(void) {
    printf("Running good function...\n");
    CWE364_Signal_Handler_Race_Condition__basic_08_good();
    printf("Finished running good function.\n");
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