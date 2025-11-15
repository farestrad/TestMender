#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

typedef struct _structSigAtomic
{
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *globalStructBad = NULL;
structSigAtomic *globalStructGood = NULL;

static void helperBad(int sig)
{
    if (globalStructBad != NULL)
    {
        globalStructBad->val = 2; // This can lead to a race condition
    }
}

static void helperGood(int sig)
{
    if (globalStructGood != NULL)
    {
        globalStructGood->val = 2; // Safely handled
    }
}

void triggerBad()
{
    signal(SIGINT, SIG_DFL);
    structSigAtomic *temp = (structSigAtomic *)malloc(sizeof(structSigAtomic));
    if (temp == NULL) exit(-1);
    globalStructBad = temp;
    globalStructBad->val = 1;
    signal(SIGINT, helperBad);
    // Potential race condition begins here
    free(globalStructBad);
    globalStructBad = NULL; // Could lead to access violation if signal fires
}

void triggerGood()
{
    signal(SIGINT, SIG_DFL);
    structSigAtomic *temp = (structSigAtomic *)malloc(sizeof(structSigAtomic));
    if (temp == NULL) exit(-1);
    globalStructGood = temp;
    globalStructGood->val = 1;
    signal(SIGINT, helperGood);
    // Safely disable signal handling
    signal(SIGINT, SIG_DFL);
    free(globalStructGood);
    globalStructGood = NULL;
}

int main()
{
    printf("Testing bad functionality...\n");
    triggerBad(); // Call bad case to expose the race condition
    printf("Finished testing bad functionality.\n");

    printf("Testing good functionality...\n");
    triggerGood(); // Call good case to avoid race condition
    printf("Finished testing good functionality.\n");

    return 0;
}