#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void printLongLongLine(int64_t data) {
    printf("%lld\n", data);
}

void printLine(const char *line) {
    printf("%s\n", line);
}

#ifndef OMITBAD

void CWE476_NULL_Pointer_Dereference__test_bad() {
    int64_t *data;
    data = NULL; // POTENTIAL FLAW: Set data to NULL

    // Attempt to use data, which may be NULL
    printLongLongLine(*data); // This may cause a NULL pointer dereference
}

#endif /* OMITBAD */

#ifndef OMITGOOD

void CWE476_NULL_Pointer_Dereference__test_goodB2G() {
    int64_t *data;
    data = NULL; // POTENTIAL FLAW: Set data to NULL

    // Check for NULL before attempting to print data
    if (data != NULL) {
        printLongLongLine(*data);
    } else {
        printLine("data is NULL"); // Expected behavior
    }
}

void CWE476_NULL_Pointer_Dereference__test_goodG2B() {
    int64_t *data;
    int64_t tmpData = 5LL;
    data = &tmpData; // FIX: Initialize data

    // Attempt to use data, which is now valid
    printLongLongLine(*data); // Expected behavior
}

void CWE476_NULL_Pointer_Dereference__test_good() {
    CWE476_NULL_Pointer_Dereference__test_goodB2G();
    CWE476_NULL_Pointer_Dereference__test_goodG2B();
}

#endif /* OMITGOOD */

int main(int argc, char *argv[]) {
#ifndef OMITGOOD
    printf("Calling good tests...\n");
    CWE476_NULL_Pointer_Dereference__test_good();
    printf("Finished good tests.\n");
#endif /* OMITGOOD */
#ifndef OMITBAD
    printf("Calling bad test...\n");
    CWE476_NULL_Pointer_Dereference__test_bad();
    printf("Finished bad test.\n");
#endif /* OMITBAD */
    return 0;
}