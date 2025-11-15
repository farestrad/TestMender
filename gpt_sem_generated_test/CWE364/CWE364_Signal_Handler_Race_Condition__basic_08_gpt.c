#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

typedef struct _structSigAtomic {
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *vulnerableStruct = NULL;

static void signalHandler(int sig) {
    if (vulnerableStruct != NULL) {
        printf("Signal handler called. Setting value to 2\n");
        vulnerableStruct->val = 2;
    }
}

void testRaceCondition() {
    structSigAtomic *gStructSigAtomic = NULL;
    signal(SIGINT, SIG_DFL);
    
    if (vulnerableStruct != NULL) {
        free(vulnerableStruct);
        vulnerableStruct = NULL;
    }

    gStructSigAtomic = (structSigAtomic*)malloc(sizeof(structSigAtomic));
    if (gStructSigAtomic == NULL) { exit(-1); }
    vulnerableStruct = gStructSigAtomic;
    vulnerableStruct->val = 1;
    
    signal(SIGINT, signalHandler);

    // Simulate a non-atomic operation that could be interrupted
    if (vulnerableStruct != NULL) {
        free(vulnerableStruct);
        vulnerableStruct = NULL;
    }

    printf("Completed non-atomic operation.\n");
}

int main() {
    printf("Starting test for signal handler race condition...\n");
    printf("Press Ctrl+C to send SIGINT...\n");

    testRaceCondition();

    // Wait for a moment to allow signal handling
    sleep(1);
    
    if (vulnerableStruct == NULL) {
        printf("Test completed without accessing freed memory.\n");
    } else {
        printf("Race condition detected! Structure is still accessible.\n");
    }

    return 0;
}