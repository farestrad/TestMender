#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printIntLine(int intNumber) {
    printf("%d\n", intNumber);
}

void printLine(const char *string) {
    printf("%s\n", string);
}

#ifndef OMITBAD

void CWE476_NULL_Pointer_Dereference__deref_after_check_02_bad() {
    if(1) {
        int *intPointer = NULL;
        if (intPointer == NULL) {
            printIntLine(*intPointer); // Vulnerable dereference after NULL check
        }
    }
}

#endif /* OMITBAD */

#ifndef OMITGOOD

static void good1() {
    if(0) {
        printLine("Benign, fixed string");
    } else {
        int *intPointer = NULL;
        if (intPointer == NULL) {
            printLine("pointer is NULL"); // Safe handling of NULL
        }
    }
}

static void good2() {
    if(1) {
        int *intPointer = NULL;
        if (intPointer == NULL) {
            printLine("pointer is NULL"); // Safe handling of NULL
        }
    }
}

#endif /* OMITGOOD */

int main(int argc, char * argv[]) {
    srand((unsigned)time(NULL));
    
#ifndef OMITGOOD
    printLine("Calling good()...");
    good1();
    good2();
    printLine("Finished good()");
#endif /* OMITGOOD */
    
#ifndef OMITBAD
    printLine("Calling bad()...");
    CWE476_NULL_Pointer_Dereference__deref_after_check_02_bad();
    printLine("Finished bad()");
#endif /* OMITBAD */

    return 0;
}