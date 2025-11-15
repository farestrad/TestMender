#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int a;
    int b;
} twoIntsStruct;

#ifndef OMITBAD
void CWE476_NULL_Pointer_Dereference__struct_53d_badSink(twoIntsStruct * data) {
    // Potential NULL pointer dereference
    printf("Data: %d, %d\n", data->a, data->b);
}
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE476_NULL_Pointer_Dereference__struct_53d_goodG2BSink(twoIntsStruct * data) {
    // Should never be NULL in this path
    printf("Data: %d, %d\n", data->a, data->b);
}

void CWE476_NULL_Pointer_Dereference__struct_53d_goodB2GSink(twoIntsStruct * data) {
    // Good check for NULL before dereference
    if (data != NULL) {
        printf("Data: %d, %d\n", data->a, data->b);
    } else {
        printf("Data is NULL\n");
    }
}
#endif /* OMITGOOD */

void testBad() {
    twoIntsStruct *data = NULL; // Bad case, data is NULL
    CWE476_NULL_Pointer_Dereference__struct_53d_badSink(data);
}

void testGoodG2B() {
    twoIntsStruct data;
    data.a = 1;
    data.b = 2;
    CWE476_NULL_Pointer_Dereference__struct_53d_goodG2BSink(&data);
}

void testGoodB2G() {
    twoIntsStruct *data = NULL; // Bad source, but tested with good sink
    CWE476_NULL_Pointer_Dereference__struct_53d_goodB2GSink(data);
}

int main() {
    printf("Running bad test...\n");
    testBad();
    
    printf("Running good test (GoodG2B)...\n");
    testGoodG2B();
    
    printf("Running good test (GoodB2G)...\n");
    testGoodB2G();
    
    return 0;
}