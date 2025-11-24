```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

typedef struct _structSigAtomic
{
    sig_atomic_t val;
}   structSigAtomic;

structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_17StructSigAtomicBad = NULL;
structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_17StructSigAtomicGood = NULL;

static void helperBad(int sig)
{
    if (CWE364_Signal_Handler_Race_Condition__basic_17StructSigAtomicBad != NULL)
    {
        CWE364_Signal_Handler_Race_Condition__basic_17StructSigAtomicBad->val = 2;
    }
}

static void helperGood(int sig)
{
    if (CWE364_Signal_Handler_Race_Condition__basic_17StructSigAtomicGood != NULL)
    {
        CWE364_Signal_Handler_Race_Condition__basic_17StructSigAtomicGood->val = 2;
    }
}

void CWE364_Signal_Handler_Race_Condition__basic_17_bad(void);
void CWE364_Signal_Handler_Race_Condition__basic_17_good(void);

void run_bad(void)
{
    printf("Running bad()...\n");
    CWE364_Signal_Handler_Race_Condition__basic_17_bad();
    printf("Finished bad()\n");
}

void run_good(void)
{
    printf("Running good()...\n");
    CWE364_Signal_Handler_Race_Condition__basic_17_good();
    printf("Finished good()\n");
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