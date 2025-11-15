```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

typedef struct _structSigAtomic {
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *testStructBad = NULL;
structSigAtomic *testStructGood = NULL;

static void badSignalHandler(int sig) {
    if (testStructBad != NULL) {
        testStructBad->val = 2;
        printf("Bad handler: Set value to %d\n", testStructBad->val);
    }
}

static void goodSignalHandler(int sig) {
    printf("Good handler: Signal received, setting value safely.\n");
}

void causeRaceCondition() {
    structSigAtomic *gStruct = (structSigAtomic *)malloc(sizeof(structSigAtomic));
    if (gStruct == NULL) {
        exit(-1);
    }
    testStructBad = gStruct;
    testStructBad->val = 1;
    signal(SIGINT, badSignalHandler); // Set bad handler

    // Simulate race condition by freeing structure
    free(testStructBad);
    testStructBad = NULL;

    // Trigger the signal, which may cause undefined behavior due to race
    raise(SIGINT);
}

void testGoodPractice() {
    structSigAtomic *gStruct = (structSigAtomic *)malloc(sizeof(structSigAtomic));
    if (gStruct == NULL) {
        exit(-1);
    }
    testStructGood = gStruct;
    testStructGood->val = 1;

    signal(SIGINT, goodSignalHandler); // Set good handler
    printf("Signal handler set to good practice.\n");

    // Safely manipulate the structure without race condition
    signal(SIGINT, SIG_DFL); // Disable handler before free
    free(testStructGood);
    testStructGood = NULL;
}

int main() {
    printf("Testing bad signal handling...\n");
    causeRaceCondition();

    printf("Testing good signal handling...\n");
    testGoodPractice();

    return 0;
}
```