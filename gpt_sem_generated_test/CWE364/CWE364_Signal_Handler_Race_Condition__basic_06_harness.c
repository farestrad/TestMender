```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

typedef struct _structSigAtomic
{
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicBad = NULL;
structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicGood = NULL;

static void helperBad(int sig)
{
    if (CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicBad != NULL)
    {
        CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicBad->val = 2;
    }
}

static void helperGood(int sig)
{
    if (CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicGood != NULL)
    {
        CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicGood->val = 2;
    }
}

void CWE364_Signal_Handler_Race_Condition__basic_06_bad(void);
void CWE364_Signal_Handler_Race_Condition__basic_06_good(void);

void run_bad(void)
{
    printf("Running bad case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_06_bad();
    printf("Finished bad case.\n");
}

void run_good(void)
{
    printf("Running good case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_06_good();
    printf("Finished good case.\n");
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