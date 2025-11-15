#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

typedef struct _structSigAtomic
{
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *badStruct = NULL;
structSigAtomic *goodStruct = NULL;

static void badSignalHandler(int sig)
{
    if (badStruct != NULL)
    {
        badStruct->val = 2;
    }
}

static void goodSignalHandler(int sig)
{
    if (goodStruct != NULL)
    {
        goodStruct->val = 2;
    }
}

void testBad()
{
    while (1)
    {
        structSigAtomic *tempStruct = NULL;
        
        signal(SIGINT, SIG_DFL);
        
        if (badStruct != NULL)
        {
            free(badStruct);
            badStruct = NULL;
        }
        
        tempStruct = (structSigAtomic *)malloc(sizeof(structSigAtomic));
        if (tempStruct == NULL) { exit(-1); }
        badStruct = tempStruct;
        badStruct->val = 1;
        
        signal(SIGINT, badSignalHandler);
        
        if (badStruct != NULL) // potential race condition here
        {
            free(badStruct);
            badStruct = NULL;
        }
        break;
    }
}

void testGood()
{
    while (1)
    {
        structSigAtomic *tempStruct = NULL;

        signal(SIGINT, SIG_DFL);

        if (goodStruct != NULL)
        {
            free(goodStruct);
            goodStruct = NULL;
        }

        tempStruct = (structSigAtomic *)malloc(sizeof(structSigAtomic));
        if (tempStruct == NULL) { exit(-1); }
        goodStruct = tempStruct;
        goodStruct->val = 1;

        signal(SIGINT, goodSignalHandler);

        // Fix: Reset handler before non-atomic operations
        signal(SIGINT, SIG_DFL);
        if (goodStruct != NULL)
        {
            free(goodStruct);
            goodStruct = NULL;
        }

        // Reassign the handler to ensure it's safe
        signal(SIGINT, goodSignalHandler);
        break;
    }
}

int main()
{
    printf("Testing bad implementation...\n");
    testBad();
    printf("Finished testing bad implementation.\n");

    printf("Testing good implementation...\n");
    testGood();
    printf("Finished testing good implementation.\n");

    return 0;
}