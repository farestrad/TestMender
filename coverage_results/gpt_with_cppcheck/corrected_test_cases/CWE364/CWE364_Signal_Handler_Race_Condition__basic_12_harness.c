// ```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>

typedef struct _structSigAtomic
{
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_12StructSigAtomicBad = NULL;
structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_12StructSigAtomicGood = NULL;

static void helperBad(int sig)
{
    if (CWE364_Signal_Handler_Race_Condition__basic_12StructSigAtomicBad != NULL)
    {
        CWE364_Signal_Handler_Race_Condition__basic_12StructSigAtomicBad->val = 2;
    }
}

static void helperGood(int sig)
{
    if (CWE364_Signal_Handler_Race_Condition__basic_12StructSigAtomicGood != NULL)
    {
        CWE364_Signal_Handler_Race_Condition__basic_12StructSigAtomicGood->val = 2;
    }
}

void CWE364_Signal_Handler_Race_Condition__basic_12_bad(void);
void CWE364_Signal_Handler_Race_Condition__basic_12_good(void);

void run_bad(void)
{
    printf("Running BAD test case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_12_bad();
    printf("Finished BAD test case.\n");
}

void run_good(void)
{
    printf("Running GOOD test case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_12_good();
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