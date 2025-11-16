```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

typedef struct _structSigAtomic
{
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_03StructSigAtomicBad = NULL;
structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_03StructSigAtomicGood = NULL;

static void helperBad(int sig)
{
    if (CWE364_Signal_Handler_Race_Condition__basic_03StructSigAtomicBad != NULL)
    {
        CWE364_Signal_Handler_Race_Condition__basic_03StructSigAtomicBad->val = 2;
    }
}

static void helperGood(int sig)
{
    if (CWE364_Signal_Handler_Race_Condition__basic_03StructSigAtomicGood != NULL)
    {
        CWE364_Signal_Handler_Race_Condition__basic_03StructSigAtomicGood->val = 2;
    }
}

#ifndef OMITBAD
void CWE364_Signal_Handler_Race_Condition__basic_03_bad(void);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE364_Signal_Handler_Race_Condition__basic_03_good(void);
#endif /* OMITGOOD */

void run_bad(void)
{
    printf("Running BAD test...\n");
    CWE364_Signal_Handler_Race_Condition__basic_03_bad();
}

void run_good(void)
{
    printf("Running GOOD test...\n");
    CWE364_Signal_Handler_Race_Condition__basic_03_good();
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