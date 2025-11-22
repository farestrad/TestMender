// ```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

typedef struct _structSigAtomic
{
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_16StructSigAtomicBad = NULL;
structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_16StructSigAtomicGood = NULL;

static void helperBad(int sig)
{
    if (CWE364_Signal_Handler_Race_Condition__basic_16StructSigAtomicBad != NULL)
    {
        CWE364_Signal_Handler_Race_Condition__basic_16StructSigAtomicBad->val = 2;
    }
}

static void helperGood(int sig)
{
    if (CWE364_Signal_Handler_Race_Condition__basic_16StructSigAtomicGood != NULL)
    {
        CWE364_Signal_Handler_Race_Condition__basic_16StructSigAtomicGood->val = 2;
    }
}

void CWE364_Signal_Handler_Race_Condition__basic_16_bad()
{
    while(1)
    {
        {
            structSigAtomic *gStructSigAtomic = NULL;
            signal(SIGINT, SIG_DFL);
            if (CWE364_Signal_Handler_Race_Condition__basic_16StructSigAtomicBad != NULL)
            {
                free(CWE364_Signal_Handler_Race_Condition__basic_16StructSigAtomicBad);
                CWE364_Signal_Handler_Race_Condition__basic_16StructSigAtomicBad = NULL;
            }
            gStructSigAtomic = (structSigAtomic*)malloc(sizeof(structSigAtomic));
            if (gStructSigAtomic == NULL) {exit(-1);}
            CWE364_Signal_Handler_Race_Condition__basic_16StructSigAtomicBad = gStructSigAtomic;
            CWE364_Signal_Handler_Race_Condition__basic_16StructSigAtomicBad->val = 1;
            signal(SIGINT, helperBad);
            if (CWE364_Signal_Handler_Race_Condition__basic_16StructSigAtomicBad != NULL)
            {
                free(CWE364_Signal_Handler_Race_Condition__basic_16StructSigAtomicBad);
                CWE364_Signal_Handler_Race_Condition__basic_16StructSigAtomicBad = NULL;
            }
        }
        break;
    }
}

static void good1()
{
    while(1)
    {
        {
            structSigAtomic *gStructSigAtomic = NULL;
            signal(SIGINT, SIG_DFL);
            if (CWE364_Signal_Handler_Race_Condition__basic_16StructSigAtomicGood != NULL)
            {
                free(CWE364_Signal_Handler_Race_Condition__basic_16StructSigAtomicGood);
                CWE364_Signal_Handler_Race_Condition__basic_16StructSigAtomicGood = 0;
            }
            gStructSigAtomic = (structSigAtomic*)malloc(sizeof(structSigAtomic));
            if (gStructSigAtomic == NULL) {exit(-1);}
            CWE364_Signal_Handler_Race_Condition__basic_16StructSigAtomicGood = gStructSigAtomic;
            CWE364_Signal_Handler_Race_Condition__basic_16StructSigAtomicGood->val = 1;
            signal(SIGINT, helperGood);
            signal(SIGINT, SIG_DFL);
            if (CWE364_Signal_Handler_Race_Condition__basic_16StructSigAtomicGood != NULL)
            {
                free(CWE364_Signal_Handler_Race_Condition__basic_16StructSigAtomicGood);
                CWE364_Signal_Handler_Race_Condition__basic_16StructSigAtomicGood = NULL;
            }
            signal(SIGINT, helperGood);
        }
        break;
    }
}

void CWE364_Signal_Handler_Race_Condition__basic_16_good()
{
    good1();
}

void run_bad(void)
{
    printf("Running BAD test case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_16_bad();
    printf("Finished BAD test case.\n");
}

void run_good(void)
{
    printf("Running GOOD test case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_16_good();
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