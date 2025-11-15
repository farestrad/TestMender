#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

typedef struct _structSigAtomic
{
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_11StructSigAtomicBad = NULL;
structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_11StructSigAtomicGood = NULL;

static void helperBad(int sig)
{
    if (CWE364_Signal_Handler_Race_Condition__basic_11StructSigAtomicBad != NULL)
    {
        CWE364_Signal_Handler_Race_Condition__basic_11StructSigAtomicBad->val = 2;
    }
}

static void helperGood(int sig)
{
    if (CWE364_Signal_Handler_Race_Condition__basic_11StructSigAtomicGood != NULL)
    {
        CWE364_Signal_Handler_Race_Condition__basic_11StructSigAtomicGood->val = 2;
    }
}

void CWE364_Signal_Handler_Race_Condition__basic_11_bad()
{
    printf("Executing bad function...\n");
    structSigAtomic *gStructSigAtomic = NULL;
    signal(SIGINT, SIG_DFL);
    free(CWE364_Signal_Handler_Race_Condition__basic_11StructSigAtomicBad);
    
    gStructSigAtomic = (structSigAtomic*)malloc(sizeof(structSigAtomic));
    if (gStructSigAtomic == NULL) {exit(-1);}
    
    CWE364_Signal_Handler_Race_Condition__basic_11StructSigAtomicBad = gStructSigAtomic;
    CWE364_Signal_Handler_Race_Condition__basic_11StructSigAtomicBad->val = 1;
    signal(SIGINT, helperBad);
    
    // Simulating a race condition
    printf("Triggering race condition...\n");
    
    usleep(50000);  // Sleep for a short duration to potentially allow the signal to interrupt

    free(CWE364_Signal_Handler_Race_Condition__basic_11StructSigAtomicBad);
    CWE364_Signal_Handler_Race_Condition__basic_11StructSigAtomicBad = NULL;

    printf("Finished executing bad function.\n");
}

void CWE364_Signal_Handler_Race_Condition__basic_11_good()
{
    printf("Executing good function...\n");
    structSigAtomic *gStructSigAtomic = NULL;
    signal(SIGINT, SIG_DFL);
    free(CWE364_Signal_Handler_Race_Condition__basic_11StructSigAtomicGood);
    
    gStructSigAtomic = (structSigAtomic*)malloc(sizeof(structSigAtomic));
    if (gStructSigAtomic == NULL) {exit(-1);}

    CWE364_Signal_Handler_Race_Condition__basic_11StructSigAtomicGood = gStructSigAtomic;
    CWE364_Signal_Handler_Race_Condition__basic_11StructSigAtomicGood->val = 1;
    signal(SIGINT, helperGood);
    
    signal(SIGINT, SIG_DFL);  // Fix: Disable handler while performing non-atomic operations

    free(CWE364_Signal_Handler_Race_Condition__basic_11StructSigAtomicGood);
    CWE364_Signal_Handler_Race_Condition__basic_11StructSigAtomicGood = NULL;

    printf("Finished executing good function.\n");
}

int main(int argc, char *argv[])
{
    printf("Starting tests...\n");

    printf("Calling bad()...\n");
    CWE364_Signal_Handler_Race_Condition__basic_11_bad();
    
    printf("Calling good()...\n");
    CWE364_Signal_Handler_Race_Condition__basic_11_good();

    printf("Tests completed.\n");
    return 0;
}