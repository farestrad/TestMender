#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

typedef struct _structSigAtomic {
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_07StructSigAtomicBad = NULL;

static void helperBad(int sig) {
    if (CWE364_Signal_Handler_Race_Condition__basic_07StructSigAtomicBad != NULL) {
        CWE364_Signal_Handler_Race_Condition__basic_07StructSigAtomicBad->val = 2;
        printf("Signal received! Value set to %d\n", CWE364_Signal_Handler_Race_Condition__basic_07StructSigAtomicBad->val);
    }
}

void CWE364_Signal_Handler_Race_Condition__basic_07_bad() {
    structSigAtomic *gStructSigAtomic = (structSigAtomic*)malloc(sizeof(structSigAtomic));
    if (gStructSigAtomic == NULL) { exit(-1); }
    CWE364_Signal_Handler_Race_Condition__basic_07StructSigAtomicBad = gStructSigAtomic;
    CWE364_Signal_Handler_Race_Condition__basic_07StructSigAtomicBad->val = 1;

    signal(SIGINT, helperBad);
    printf("Triggering potential race condition. Press Ctrl+C\n");
    pause(); // Wait for signal
}

int main() {
    printf("Testing bad signal handler...\n");
    CWE364_Signal_Handler_Race_Condition__basic_07_bad();
    
    // Clean up (not reached if interrupted)
    if (CWE364_Signal_Handler_Race_Condition__basic_07StructSigAtomicBad != NULL) {
        free(CWE364_Signal_Handler_Race_Condition__basic_07StructSigAtomicBad);
    }
    return 0;
}