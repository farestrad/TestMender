// ```c
#include "std_testcase.h"
#include <signal.h>

typedef struct _structSigAtomic
{
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_13StructSigAtomicBad = NULL;
structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_13StructSigAtomicGood = NULL;

void CWE364_Signal_Handler_Race_Condition__basic_13_bad(void);
void CWE364_Signal_Handler_Race_Condition__basic_13_good(void);

static void run_bad(void)
{
    printf("Running BAD test case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_13_bad();
    printf("Finished BAD test case.\n");
}

static void run_good(void)
{
    printf("Running GOOD test case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_13_good();
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