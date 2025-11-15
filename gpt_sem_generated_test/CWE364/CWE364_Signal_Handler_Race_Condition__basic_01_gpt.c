#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct _structSigAtomic
{
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *gStructSigAtomicBad = NULL;
structSigAtomic *gStructSigAtomicGood = NULL;

static void helperBad(int sig)
{
    if (gStructSigAtomicBad != NULL)
    {
        gStructSigAtomicBad->val = 2;
    }
}

static void helperGood(int sig)
{
    if (gStructSigAtomicGood != NULL)
    {
        gStructSigAtomicGood->val = 2;
    }
}

void CWE364_Signal_Handler_Race_Condition__basic_01_bad()
{
    structSigAtomic *localStruct = NULL;
    signal(SIGINT, SIG_DFL);
    
    if (gStructSigAtomicBad != NULL)
    {
        free(gStructSigAtomicBad);
        gStructSigAtomicBad = NULL;
    }
    
    localStruct = (structSigAtomic*)malloc(sizeof(structSigAtomic));
    if (localStruct == NULL) { exit(-1); }
    
    gStructSigAtomicBad = localStruct;
    gStructSigAtomicBad->val = 1;
    signal(SIGINT, helperBad);
    
    if (gStructSigAtomicBad != NULL)
    {
        free(gStructSigAtomicBad);
        gStructSigAtomicBad = NULL;
    }
}

void good1()
{
    structSigAtomic *localStruct = NULL;
    signal(SIGINT, SIG_DFL);
    
    if (gStructSigAtomicGood != NULL)
    {
        free(gStructSigAtomicGood);
        gStructSigAtomicGood = NULL;
    }
    
    localStruct = (structSigAtomic*)malloc(sizeof(structSigAtomic));
    if (localStruct == NULL) { exit(-1); }
    
    gStructSigAtomicGood = localStruct;
    gStructSigAtomicGood->val = 1;
    signal(SIGINT, helperGood);
    
    signal(SIGINT, SIG_DFL);
    
    if (gStructSigAtomicGood != NULL)
    {
        free(gStructSigAtomicGood);
        gStructSigAtomicGood = NULL;
    }
    
    signal(SIGINT, helperGood);
}

int main(int argc, char *argv[])
{
    printf("Testing bad implementation...\n");
    CWE364_Signal_Handler_Race_Condition__basic_01_bad();
    printf("Finished bad implementation.\n");
    
    printf("Testing good implementation...\n");
    good1();
    printf("Finished good implementation.\n");
    
    return 0;
}