#include <stdio.h>
#include <stdint.h>

void printLongLongLine(int64_t value) {
    printf("%lld\n", value);
}

void printLine(const char* message) {
    printf("%s\n", message);
}

#ifndef OMITBAD

void CWE476_NULL_Pointer_Dereference__int64_t_51b_badSink(int64_t * data) {
    /* POTENTIAL FLAW: Attempt to use data, which may be NULL */
    printLongLongLine(*data);
}

void test_bad() {
    int64_t *data = NULL; // setting data to NULL
    printf("Testing BAD case...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_51b_badSink(data);
}

#endif /* OMITBAD */

#ifndef OMITGOOD

void CWE476_NULL_Pointer_Dereference__int64_t_51b_goodG2BSink(int64_t * data) {
    /* POTENTIAL FLAW: Attempt to use data, which may be NULL */
    printLongLongLine(*data);
}

void CWE476_NULL_Pointer_Dereference__int64_t_51b_goodB2GSink(int64_t * data) {
    /* FIX: Check for NULL before attempting to print data */
    if (data != NULL) {
        printLongLongLine(*data);
    } else {
        printLine("data is NULL");
    }
}

void test_goodG2B() {
    int64_t value = 5; // Proper initialization
    int64_t *data = &value;
    printf("Testing GOOD G2B case...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_51b_goodG2BSink(data);
}

void test_goodB2G() {
    int64_t *data = NULL; // setting data to NULL
    printf("Testing GOOD B2G case...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_51b_goodB2GSink(data);
}

#endif /* OMITGOOD */

int main() {
    #ifndef OMITBAD
    test_bad();
    #endif /* OMITBAD */

    #ifndef OMITGOOD
    test_goodG2B();
    test_goodB2G();
    #endif /* OMITGOOD */

    return 0;
}