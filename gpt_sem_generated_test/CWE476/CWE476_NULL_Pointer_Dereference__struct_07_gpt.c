#include <stdio.h>
#include <stdlib.h>

/* Struct definition */
typedef struct {
    int intOne;
    int intTwo;
} twoIntsStruct;

/* Function prototypes */
void CWE476_NULL_Pointer_Dereference__struct_07_bad();
void CWE476_NULL_Pointer_Dereference__struct_07_good();

static int staticFive = 5;

void CWE476_NULL_Pointer_Dereference__struct_07_bad() {
    twoIntsStruct * data;
    if(staticFive == 5) {
        /* POTENTIAL FLAW: Set data to NULL */
        data = NULL;
    }
    if(staticFive == 5) {
        /* POTENTIAL FLAW: Attempt to use data, which may be NULL */
        printf("%d\n", data->intOne); // This will cause a NULL pointer dereference
    }
}

void CWE476_NULL_Pointer_Dereference__struct_07_good() {
    twoIntsStruct * data;
    twoIntsStruct tmpData;

    /* Good Source: Initialize data */
    tmpData.intOne = 1;
    tmpData.intTwo = 2;
    data = &tmpData;

    if(staticFive == 5) {
        /* Check for NULL before attempting to print data */
        if (data != NULL) {
            printf("%d\n", data->intOne); // Safe access
        } else {
            printf("data is NULL\n");
        }
    }
}

int main() {
    printf("Testing the bad function...\n");
    CWE476_NULL_Pointer_Dereference__struct_07_bad();
    
    printf("Testing the good function...\n");
    CWE476_NULL_Pointer_Dereference__struct_07_good();
    
    return 0;
}