```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

typedef struct _structSigAtomic
{
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_17StructSigAtomicBad = NULL;

static void helperBad(int sig)
{
    if (CWE364_Signal_Handler_Race_Condition__basic_17StructSigAtomicBad != NULL)
    {
        CWE364_Signal_Handler_Race_Condition__basic_17StructSigAtomicBad->val = 2;
    }
}

void CWE364_Signal_Handler_Race_Condition__basic_17_bad()
{
    int j;
    for (j = 0; j < 1; j++)
    {
        structSigAtomic *gStructSigAtomic = NULL;
        signal(SIGINT, SIG_DFL);
        if (CWE364_Signal_Handler_Race_Condition__basic_17StructSigAtomicBad != NULL)
        {
            free(CWE364_Signal_Handler_Race_Condition__basic_17StructSigAtomicBad);
            CWE364_Signal_Handler_Race_Condition__basic_17StructSigAtomicBad = NULL;
        }
        gStructSigAtomic = (structSigAtomic*)malloc(sizeof(structSigAtomic));
        if (gStructSigAtomic == NULL) { exit(-1); }
        CWE364_Signal_Handler_Race_Condition__basic_17StructSigAtomicBad = gStructSigAtomic;
        CWE364_Signal_Handler_Race_Condition__basic_17StructSigAtomicBad->val = 1;
        signal(SIGINT, helperBad);
        
        // Simulate a potential race condition
        kill(getpid(), SIGINT);
        
        if (CWE364_Signal_Handler_Race_Condition__basic_17StructSigAtomicBad != NULL)
        {
            printf("Before free: val = %d\n", CWE364_Signal_Handler_Race_Condition__basic_17StructSigAtomicBad->val);
            free(CWE364_Signal_Handler_Race_Condition__basic_17StructSigAtomicBad);
            CWE364_Signal_Handler_Race_Condition__basic_17StructSigAtomicBad = NULL;
        }
        else
        {
            printf("CWE364_Signal_Handler_Race_Condition__basic_17StructSigAtomicBad is NULL!\n");
        }
    }
}

int main(int argc, char *argv[])
{
    printf("Starting bad() test...\n");
    CWE364_Signal_Handler_Race_Condition__basic_17_bad();
    printf("Finished bad() test.\n");
    return 0;
}
```