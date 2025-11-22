// ```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "std_testcase.h"

typedef struct _structSigAtomic
{
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_04StructSigAtomicBad = NULL;
structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_04StructSigAtomicGood = NULL;

void CWE364_Signal_Handler_Race_Condition__basic_04_bad(void);
void CWE364_Signal_Handler_Race_Condition__basic_04_good(void);

static void run_bad(void)
{
    printf("Running bad()...\n");
    CWE364_Signal_Handler_Race_Condition__basic_04_bad();
    printf("Finished running bad().\n");
}

static void run_good(void)
{
    printf("Running good()...\n");
    CWE364_Signal_Handler_Race_Condition__basic_04_good();
    printf("Finished running good().\n");
}

int main(void)
{
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
// ```