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

static void helperBad(int sig) {
    if (testStructBad != NULL) {
        testStructBad->val = 2;
    }
}

static void helperGood(int sig) {
    if (testStructGood != NULL) {
        testStructGood->val = 2;
    }
}

void testBad() {
    structSigAtomic *gStructSigAtomic = NULL;
    signal(SIGINT, SIG_DFL);
    gStructSigAtomic = (structSigAtomic*)malloc(sizeof(structSigAtomic));
    if (gStructSigAtomic == NULL) { exit(-1); }
    testStructBad = gStructSigAtomic;
    testStructBad->val = 1;

    signal(SIGINT, helperBad);
    printf("Triggering a potential race condition...\n");
    if (testStructBad != NULL) {
        free(testStructBad);
        testStructBad = NULL;
    }
}

void testGood() {
    structSigAtomic *gStructSigAtomic = NULL;
    signal(SIGINT, SIG_DFL);
    gStructSigAtomic = (structSigAtomic*)malloc(sizeof(structSigAtomic));
    if (gStructSigAtomic == NULL) { exit(-1); }
    testStructGood = gStructSigAtomic;
    testStructGood->val = 1;

    signal(SIGINT, helperGood);
    printf("Temporarily disabling signal handler to avoid race condition...\n");
    signal(SIGINT, SIG_DFL);
    if (testStructGood != NULL) {
        free(testStructGood);
        testStructGood = NULL;
    }
    signal(SIGINT, helperGood);
}

int main() {
    printf("Running bad test case...\n");
    testBad();
    printf("Running good test case...\n");
    testGood();
    return 0;
}
```