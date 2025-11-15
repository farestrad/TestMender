```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

typedef struct _structSigAtomic {
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *testStruct = NULL;

static void signalHandler(int sig) {
    if (testStruct != NULL) {
        testStruct->val = 2; // Potential race condition
        printf("Signal handler executed. Updated val to %d\n", testStruct->val);
    }
}

void testRaceCondition() {
    signal(SIGINT, signalHandler); // Setting the signal handler
    structSigAtomic *localStruct = (structSigAtomic*)malloc(sizeof(structSigAtomic));
    if (localStruct == NULL) { exit(-1); }
    
    testStruct = localStruct;
    testStruct->val = 1; // Initial value
    printf("Initial value set to %d\n", testStruct->val);

    sleep(1); // Wait for a moment to simulate a potential race condition

    // Simulate freeing the memory while a signal might come
    free(testStruct);
    testStruct = NULL;

    // Perform a check to see if the handler corrupted the state (this will likely not happen due to null check)
    if (testStruct == NULL) {
        printf("testStruct is NULL after free, no race condition occurred.\n");
    } else {
        printf("Race condition detected, val is %d\n", testStruct->val);
    }
}

int main() {
    printf("Testing signal handler race condition...\n");
    testRaceCondition();
    
    return 0;
}
```