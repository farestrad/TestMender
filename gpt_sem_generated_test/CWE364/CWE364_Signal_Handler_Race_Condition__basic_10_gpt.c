#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct _structSigAtomic
{
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *badStruct = NULL;
structSigAtomic *goodStruct = NULL;

static void handlerBad(int sig)
{
    if (badStruct != NULL)
    {
        badStruct->val = 2;
    }
}

static void handlerGood(int sig)
{
    if (goodStruct != NULL)
    {
        goodStruct->val = 2;
    }
}

void testBad()
{
    printf("Executing Bad Test...\n");
    signal(SIGINT, SIG_DFL);
    
    badStruct = (structSigAtomic*)malloc(sizeof(structSigAtomic));
    if (badStruct == NULL) { exit(-1); }
    badStruct->val = 1;

    signal(SIGINT, handlerBad);
    
    // Simulate a race condition
    if (badStruct != NULL)
    {
        free(badStruct);
        badStruct = NULL;
    }
    printf("Bad Test Completed.\n");
}

void testGood()
{
    printf("Executing Good Test...\n");
    signal(SIGINT, SIG_DFL);
    
    goodStruct = (structSigAtomic*)malloc(sizeof(structSigAtomic));
    if (goodStruct == NULL) { exit(-1); }
    goodStruct->val = 1;

    signal(SIGINT, handlerGood);
    
    // Properly handling signal
    signal(SIGINT, SIG_DFL);
    if (goodStruct != NULL)
    {
        free(goodStruct);
        goodStruct = NULL;
    }
    printf("Good Test Completed.\n");
}

int main()
{
    printf("Starting Tests...\n");
    
    testBad();
    raise(SIGINT); // Trigger the bad handler

    testGood();
    
    printf("All Tests Completed.\n");
    return 0;
}