```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

typedef struct _structSigAtomic {
    sig_atomic_t val;
} structSigAtomic;

// Global variables used to simulate signal handler targets.
structSigAtomic *testStructBad = NULL;
structSigAtomic *testStructGood = NULL;

// Helper functions for signal handling
static void helperBad(int sig) {
    if (testStructBad != NULL) {
        testStructBad->val = 2; // Potential race condition here
    }
}

static void helperGood(int sig) {
    // This handler does nothing in this scenario
}

// Function to test the bad case (CWE-364 vulnerability)
void testBad() {
    structSigAtomic *temp = (structSigAtomic *)malloc(sizeof(structSigAtomic));
    if (temp == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }
    testStructBad = temp;
    testStructBad->val = 1;

    signal(SIGINT, helperBad); // Set the signal handler
    printf("Press Ctrl+C to trigger the signal and simulate a race condition...\n");
    pause(); // Wait for a signal
    printf("Finished bad test.\n");

    if (testStructBad != NULL) {
        free(testStructBad);
        testStructBad = NULL;
    }
}

// Function to test the good case (No race condition)
void testGood() {
    if (testStructGood != NULL) {
        free(testStructGood);
        testStructGood = NULL;
    }

    structSigAtomic *temp = (structSigAtomic *)malloc(sizeof(structSigAtomic));
    if (temp == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }
    testStructGood = temp;
    testStructGood->val = 1;

    signal(SIGINT, helperGood); // Set the signal handler
    printf("Press Ctrl+C to trigger the signal (no race condition)...\n");
    pause(); // Wait for a signal
    printf("Finished good test.\n");

    if (testStructGood != NULL) {
        free(testStructGood);
        testStructGood = NULL;
    }
}

int main() {
    srand((unsigned)time(NULL));

    // Testing bad case
    printf("Starting bad test (CWE-364)...\n");
    testBad();

    // Testing good case
    printf("Starting good test (no race condition)...\n");
    testGood();

    return 0;
}
```