#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

typedef struct _structSigAtomic
{
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *sharedStruct = NULL;

static void signalHandler(int sig)
{
    if (sharedStruct != NULL)
    {
        sharedStruct->val = 2;
        printf("Signal handler executed, struct value changed to %d\n", sharedStruct->val);
    }
}

void testRaceCondition()
{
    sharedStruct = (structSigAtomic*)malloc(sizeof(structSigAtomic));
    if (sharedStruct == NULL) { exit(-1); }
    sharedStruct->val = 1;
    printf("Initial struct value set to %d\n", sharedStruct->val);
    
    signal(SIGINT, signalHandler);
    
    // Simulating potential race condition
    printf("Press Ctrl+C to trigger the signal handler...\n");
    sleep(5); // Allow time to trigger the signal

    free(sharedStruct);
    sharedStruct = NULL;
}

int main()
{
    printf("Testing race condition vulnerability...\n");
    testRaceCondition();
    
    if (sharedStruct == NULL) {
        printf("Memory successfully freed and pointer set to NULL.\n");
    } else {
        printf("Potential race condition: struct still exists with value %d\n", sharedStruct->val);
    }
    
    return 0;
}