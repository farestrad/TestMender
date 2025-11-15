```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

typedef struct _structSigAtomic
{
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_14StructSigAtomicBad = NULL;

static void helperBad(int sig)
{
    if (CWE364_Signal_Handler_Race_Condition__basic_14StructSigAtomicBad != NULL)
    {
        CWE364_Signal_Handler_Race_Condition__basic_14StructSigAtomicBad->val = 2;
    }
}

void CWE364_Signal_Handler_Race_Condition__basic_14_bad()
{
    structSigAtomic *gStructSigAtomic = (structSigAtomic*)malloc(sizeof(structSigAtomic));
    if (gStructSigAtomic == NULL) { exit(-1); }
    
    CWE364_Signal_Handler_Race_Condition__basic_14StructSigAtomicBad = gStructSigAtomic;
    CWE364_Signal_Handler_Race_Condition__basic_14StructSigAtomicBad->val = 1;
    
    signal(SIGINT, helperBad);
    
    // Trigger the race condition
    free(CWE364_Signal_Handler_Race_Condition__basic_14StructSigAtomicBad);
    CWE364_Signal_Handler_Race_Condition__basic_14StructSigAtomicBad = NULL;
    
    // Waiting to potentially receive a signal
    printf("Waiting for signal. Press Ctrl+C to trigger...\n");
    pause(); // Wait for signal
}

int main(int argc, char *argv[])
{
    printf("Starting bad test case...\n");
    CWE364_Signal_Handler_Race_Condition__basic_14_bad();
    printf("Finished bad test case.\n");
    
    return 0;
}
```