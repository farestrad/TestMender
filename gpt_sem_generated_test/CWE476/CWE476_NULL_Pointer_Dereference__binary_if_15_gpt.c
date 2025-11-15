#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int intOne;
    int intTwo;
} twoIntsStruct;

void printLine(const char *line) {
    printf("%s\n", line);
}

void CWE476_NULL_Pointer_Dereference__binary_if_15_bad() {
    switch(6) {
        case 6: {
            twoIntsStruct *twoIntsStructPointer = NULL;
            // FLAW: This will dereference a NULL pointer
            if ((twoIntsStructPointer != NULL) & (twoIntsStructPointer->intOne == 5)) {
                printLine("intOne == 5");
            } else {
                printLine("Bad case: Dereference NULL prevented.");
            }
        }
        break;
        default:
            printLine("Benign, fixed string");
            break;
    }
}

void CWE476_NULL_Pointer_Dereference__binary_if_15_good() {
    switch(6) {
        case 6: {
            twoIntsStruct *twoIntsStructPointer = NULL;
            // FIX: Proper null check
            if ((twoIntsStructPointer != NULL) && (twoIntsStructPointer->intOne == 5)) {
                printLine("intOne == 5");
            } else {
                printLine("Good case: Proper null check executed.");
            }
        }
        break;
        default:
            printLine("Benign, fixed string");
            break;
    }
}

int main(int argc, char * argv[]) {
    printLine("Testing bad function:");
    CWE476_NULL_Pointer_Dereference__binary_if_15_bad();
    
    printLine("Testing good function:");
    CWE476_NULL_Pointer_Dereference__binary_if_15_good();
    
    return 0;
}