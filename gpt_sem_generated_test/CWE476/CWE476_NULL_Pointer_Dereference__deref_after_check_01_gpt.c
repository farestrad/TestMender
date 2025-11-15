#include <stdio.h>
#include <stdlib.h>

void printIntLine(int intToPrint) {
    printf("%d\n", intToPrint);
}

void printLine(const char *line) {
    printf("%s\n", line);
}

#ifndef OMITBAD

void CWE476_NULL_Pointer_Dereference__deref_after_check_01_bad() {
    int *intPointer = NULL;
    if (intPointer == NULL) {
        printIntLine(*intPointer); // Vulnerable dereference
    }
}

#endif /* OMITBAD */

#ifndef OMITGOOD

static void good1() {
    int *intPointer = NULL;
    if (intPointer == NULL) {
        printLine("pointer is NULL"); // Safe handling
    }
}

#endif /* OMITGOOD */

int main(int argc, char * argv[]) {
    // Test the bad case
    printf("Calling bad function...\n");
    CWE476_NULL_Pointer_Dereference__deref_after_check_01_bad();
    printf("Finished calling bad function.\n");

    // Test the good case
    printf("Calling good function...\n");
    good1();
    printf("Finished calling good function.\n");

    return 0;
}