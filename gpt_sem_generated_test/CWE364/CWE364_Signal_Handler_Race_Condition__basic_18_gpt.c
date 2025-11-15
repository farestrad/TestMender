#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

typedef struct _structSigAtomic
{
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *gStructSigAtomic = NULL;

static void helperBad(int sig)
{
    if (gStructSigAtomic != NULL)
    {
        gStructSigAtomic->val = 2;
    }
}

static void helperGood(int sig)
{
    if (gStructSigAtomic != NULL)
    {
        gStructSigAtomic->val = 2;
    }
}

void testBad()
{
    signal(SIGINT, SIG_DFL);
    
    gStructSigAtomic = (structSigAtomic*)malloc(sizeof(structSigAtomic));
    if (gStructSigAtomic == NULL) {exit(-1);}
    gStructSigAtomic->val = 1;
    
    // Trigger signal delivery, should cause potential race condition
    signal(SIGINT, helperBad);
    raise(SIGINT); // Simulate sending SIGINT
    
    free(gStructSigAtomic);
    gStructSigAtomic = NULL;
    printf("Test Bad: Done\n");
}

void testGood()
{
    signal(SIGINT, SIG_DFL);
    
    gStructSigAtomic = (structSigAtomic*)malloc(sizeof(structSigAtomic));
    if (gStructSigAtomic == NULL) {exit(-1);}
    gStructSigAtomic->val = 1;

    // Temporarily disable the signal handler to avoid race condition
    signal(SIGINT, SIG_DFL);
    
    // Restore signal handler
    signal(SIGINT, helperGood);
    raise(SIGINT); // Simulate sending SIGINT
    
    free(gStructSigAtomic);
    gStructSigAtomic = NULL;
    printf("Test Good: Done\n");
}

int main()
{
    printf("Running bad test...\n");
    testBad();
    
    printf("Running good test...\n");
    testGood();
    
    return 0;
}