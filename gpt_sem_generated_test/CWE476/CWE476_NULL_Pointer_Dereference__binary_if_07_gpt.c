#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct _twoIntsStruct {
    int intOne;
    int intTwo;
} twoIntsStruct;

static int staticFive = 5;

void CWE476_NULL_Pointer_Dereference__test_case() {
    if(staticFive == 5) {
        twoIntsStruct *twoIntsStructPointer = NULL;
        // FLAW: Using a single & in the if statement,
        if ((twoIntsStructPointer != NULL) & (twoIntsStructPointer->intOne == 5)) {
            printf("intOne == 5\n");
        } else {
            printf("Pointer is NULL or intOne is not 5\n");
        }
    }
}

void good_case() {
    if(staticFive != 5) {
        printf("Benign, fixed string\n");
    } else {
        twoIntsStruct *twoIntsStructPointer = NULL;
        // FIX: Use && in the if statement to prevent dereferencing a NULL pointer
        if ((twoIntsStructPointer != NULL) && (twoIntsStructPointer->intOne == 5)) {
            printf("intOne == 5\n");
        } else {
            printf("Pointer is NULL or intOne is not 5\n");
        }
    }
}

int main(int argc, char *argv[]) {
    srand((unsigned)time(NULL));
    printf("Running test case...\n");
    CWE476_NULL_Pointer_Dereference__test_case();
    printf("Running good case...\n");
    good_case();
    return 0;
}