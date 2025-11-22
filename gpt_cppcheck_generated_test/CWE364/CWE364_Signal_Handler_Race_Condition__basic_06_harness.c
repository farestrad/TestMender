```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "std_testcase.h"

typedef struct _structSigAtomic {
    sig_atomic_t val;
} structSigAtomic;

structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicBad = NULL;
structSigAtomic *CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicGood = NULL;

static void helperBad(int sig) {
    if (CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicBad != NULL) {
        CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicBad->val = 2;
    }
}

static void helperGood(int sig) {
    if (CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicGood != NULL) {
        CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicGood->val = 2;
    }
}

static const int STATIC_CONST_FIVE = 5;

#ifndef OMITBAD
void CWE364_Signal_Handler_Race_Condition__basic_06_bad() {
    if (STATIC_CONST_FIVE == 5) {
        structSigAtomic *gStructSigAtomic = NULL;
        signal(SIGINT, SIG_DFL);
        if (CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicBad != NULL) {
            free(CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicBad);
            CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicBad = NULL;
        }
        gStructSigAtomic = (structSigAtomic *)malloc(sizeof(structSigAtomic));
        if (gStructSigAtomic == NULL) { exit(-1); }
        CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicBad = gStructSigAtomic;
        CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicBad->val = 1;
        signal(SIGINT, helperBad);
        printf("Executing bad test case...\n");
        if (CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicBad != NULL) {
            free(CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicBad);
            CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicBad = NULL;
        }
    }
}
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE364_Signal_Handler_Race_Condition__basic_06_good() {
    if (STATIC_CONST_FIVE != 5) {
        printf("This branch should not be taken.\n");
    } else {
        structSigAtomic *gStructSigAtomic = NULL;
        signal(SIGINT, SIG_DFL);
        if (CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicGood != NULL) {
            free(CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicGood);
            CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicGood = 0;
        }
        gStructSigAtomic = (structSigAtomic *)malloc(sizeof(structSigAtomic));
        if (gStructSigAtomic == NULL) { exit(-1); }
        CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicGood = gStructSigAtomic;
        CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicGood->val = 1;
        signal(SIGINT, helperGood);
        printf("Executing good test case...\n");
        signal(SIGINT, SIG_DFL);
        if (CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicGood != NULL) {
            free(CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicGood);
            CWE364_Signal_Handler_Race_Condition__basic_06StructSigAtomicGood = NULL;
        }
        signal(SIGINT, helperGood);
    }
}
#endif /* OMITGOOD */

void run_bad(void) {
    CWE364_Signal_Handler_Race_Condition__basic_06_bad();
}

void run_good(void) {
    CWE364_Signal_Handler_Race_Condition__basic_06_good();
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```