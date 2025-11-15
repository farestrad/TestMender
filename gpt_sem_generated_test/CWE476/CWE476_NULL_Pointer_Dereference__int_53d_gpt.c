#include <stdio.h>

void printIntLine(int data) {
    printf("%d\n", data);
}

void printLine(const char* str) {
    printf("%s\n", str);
}

#ifndef OMITBAD

void CWE476_NULL_Pointer_Dereference__int_53d_badSink(int * data) {
    printIntLine(*data);
}

void testBad() {
    int *data = NULL;  // Set data to NULL
    printf("Testing Bad Sink...\n");
    CWE476_NULL_Pointer_Dereference__int_53d_badSink(data);
}

#endif /* OMITBAD */

#ifndef OMITGOOD

void CWE476_NULL_Pointer_Dereference__int_53d_goodG2BSink(int * data) {
    printIntLine(*data);
}

void CWE476_NULL_Pointer_Dereference__int_53d_goodB2GSink(int * data) {
    if (data != NULL) {
        printIntLine(*data);
    } else {
        printLine("data is NULL");
    }
}

void testGoodG2B() {
    int value = 42;  // Initialize data
    int *data = &value;
    printf("Testing Good Source with Bad Sink...\n");
    CWE476_NULL_Pointer_Dereference__int_53d_goodG2BSink(data);
}

void testGoodB2G() {
    int *data = NULL;  // Set data to NULL
    printf("Testing Bad Source with Good Sink...\n");
    CWE476_NULL_Pointer_Dereference__int_53d_goodB2GSink(data);
}

#endif /* OMITGOOD */

int main() {
    #ifndef OMITBAD
    testBad();
    #endif /* OMITBAD */

    #ifndef OMITGOOD
    testGoodG2B();
    testGoodB2G();
    #endif /* OMITGOOD */

    return 0;
}