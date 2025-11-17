```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

typedef struct _structSigAtomic
{
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_07StructSigAtomicBad = NULL;
structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_07StructSigAtomicGood = NULL;

static void helperBad(int sig)
{
    if (CWE364_Signal_Handler_Race_Condition__basic_07StructSigAtomicBad != NULL)
    {
        CWE364_Signal_Handler_Race_Condition__basic_07StructSigAtomicBad->val = 2;
    }
}

static void helperGood(int sig)
{
    if (CWE364_Signal_Handler_Race_Condition__basic_07StructSigAtomicGood != NULL)
    {
        CWE364_Signal_Handler_Race_Condition__basic_07StructSigAtomicGood->val = 2;
    }
}

static int staticFive = 5;

#ifndef OMITBAD
void CWE364_Signal_Handler_Race_Condition__basic_07_bad(void);
#endif

#ifndef OMITGOOD
void CWE364_Signal_Handler_Race_Condition__basic_07_good(void);
#endif

void run_bad(void) {
    printf("Running bad() test...\n");
    CWE364_Signal_Handler_Race_Condition__basic_07_bad();
    printf("Finished bad() test.\n");
}

void run_good(void) {
    printf("Running good() test...\n");
    CWE364_Signal_Handler_Race_Condition__basic_07_good();
    printf("Finished good() test.\n");
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