// ```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

typedef struct _structSigAtomic
{
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_01StructSigAtomicBad = NULL;
structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_01StructSigAtomicGood = NULL;

static void helperBad(int sig)
{
    if (CWE364_Signal_Handler_Race_Condition__basic_01StructSigAtomicBad != NULL)
    {
        CWE364_Signal_Handler_Race_Condition__basic_01StructSigAtomicBad->val = 2;
    }
}

static void helperGood(int sig)
{
    if (CWE364_Signal_Handler_Race_Condition__basic_01StructSigAtomicGood != NULL)
    {
        CWE364_Signal_Handler_Race_Condition__basic_01StructSigAtomicGood->val = 2;
    }
}

#ifndef OMITBAD
void CWE364_Signal_Handler_Race_Condition__basic_01_bad(void);
#endif

#ifndef OMITGOOD
void CWE364_Signal_Handler_Race_Condition__basic_01_good(void);
#endif

void run_bad(void) {
    printf("Running BAD test case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_01_bad();
    printf("Finished BAD test case.\n");
}

void run_good(void) {
    printf("Running GOOD test case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_01_good();
    printf("Finished GOOD test case.\n");
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