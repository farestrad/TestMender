```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

typedef struct _structSigAtomic
{
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_05StructSigAtomicBad = NULL;

static void helperBad(int sig)
{
    if (CWE364_Signal_Handler_Race_Condition__basic_05StructSigAtomicBad != NULL)
    {
        CWE364_Signal_Handler_Race_Condition__basic_05StructSigAtomicBad->val = 2;
    }
}

void CWE364_Signal_Handler_Race_Condition__basic_05_bad()
{
    structSigAtomic *gStructSigAtomic = (structSigAtomic*)malloc(sizeof(structSigAtomic));
    if (gStructSigAtomic == NULL) { exit(-1); }
    
    CWE364_Signal_Handler_Race_Condition__basic_05StructSigAtomicBad = gStructSigAtomic;
    CWE364_Signal_Handler_Race_Condition__basic_05StructSigAtomicBad->val = 1;

    signal(SIGINT, helperBad);

    // Simulate non-atomic operation
    free(CWE364_Signal_Handler_Race_Condition__basic_05StructSigAtomicBad);
    CWE364_Signal_Handler_Race_Condition__basic_05StructSigAtomicBad = NULL;

    printf("CWE364 Bad Test Finished.\n");
}

void triggerSignal()
{
    raise(SIGINT);
}

int main()
{
    printf("Starting bad test...\n");
    CWE364_Signal_Handler_Race_Condition__basic_05_bad();
    printf("Triggering signal...\n");
    triggerSignal();

    return 0;
}
```