#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Function declarations */
static int staticReturnsTrue();
static int staticReturnsFalse();

static int staticReturnsTrue() {
    return 1;
}

static int staticReturnsFalse() {
    return 0;
}

void CWE476_NULL_Pointer_Dereference__deref_after_check() {
    if(staticReturnsTrue()) {
        int *intPointer = NULL;
        if (intPointer == NULL) {
            // Bad practice: dereferencing after checking for NULL
            printf("Dereferencing NULL pointer: %d\n", *intPointer); // This will cause a crash
        }
    }
}

void testGood() {
    if(staticReturnsFalse()) {
        // This won't execute
        printf("This line should not be executed.\n");
    } else {
        int *intPointer = NULL;
        if (intPointer == NULL) {
            printf("pointer is NULL, not dereferencing\n");
        }
    }
}

int main() {
    // Seed randomness for the test cases
    srand((unsigned)time(NULL));

    printf("Running bad test...\n");
    CWE476_NULL_Pointer_Dereference__deref_after_check();
    
    printf("Running good test...\n");
    testGood();

    return 0;
}