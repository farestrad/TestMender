```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

typedef struct _structSigAtomic
{
    volatile sig_atomic_t val;
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

void testBadCase()
{
    signal(SIGINT, SIG_DFL);
    if (badStruct != NULL)
    {
        free(badStruct);
        badStruct = NULL;
    }
    badStruct = (structSigAtomic*)malloc(sizeof(structSigAtomic));
    if (badStruct == NULL) { exit(-1); }
    badStruct->val = 1;
    signal(SIGINT, badSignalHandler);

    // Simulating a race condition
    if (badStruct != NULL)
    {
        free(badStruct);
        badStruct = NULL;
    }
    printf("Finished bad case test\n");
}

void testGoodCase()
{
    signal(SIGINT, SIG_DFL);
    if (goodStruct != NULL)
    {
        free(goodStruct);
        goodStruct = NULL;
    }
    goodStruct = (structSigAtomic*)malloc(sizeof(structSigAtomic));
    if (goodStruct == NULL) { exit(-1); }
    goodStruct->val = 1;
    signal(SIGINT, goodSignalHandler);

    // FIX: Disable signal while operating on non-atomic operations
    signal(SIGINT, SIG_DFL);
    if (goodStruct != NULL)
    {
        free(goodStruct);
        goodStruct = NULL;
    }
    signal(SIGINT, goodSignalHandler);
    printf("Finished good case test\n");
}

int main()
{
    printf("Starting tests...\n");
    // Uncomment the following line to trigger bad behavior
    // testBadCase();
    
    // Running the good case
    testGoodCase();

    return 0;
}
```