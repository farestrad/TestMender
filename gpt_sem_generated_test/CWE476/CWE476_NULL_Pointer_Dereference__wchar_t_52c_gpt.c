#include <stdio.h>
#include <stdlib.h>

void printWcharLine(wchar_t data) {
    wprintf(L"%lc\n", data);
}

void printLine(const char *message) {
    printf("%s\n", message);
}

void CWE476_NULL_Pointer_Dereference__wchar_t_52c_badSink(wchar_t * data) {
    printWcharLine(data[0]);
}

void CWE476_NULL_Pointer_Dereference__wchar_t_52c_goodG2BSink(wchar_t * data) {
    printWcharLine(data[0]);
}

void CWE476_NULL_Pointer_Dereference__wchar_t_52c_goodB2GSink(wchar_t * data) {
    if (data != NULL) {
        printWcharLine(data[0]);
    } else {
        printLine("data is NULL");
    }
}

int main() {
    wchar_t *nullData = NULL;
    wchar_t validData[] = L'H';

    // Test bad sink with NULL pointer
    printf("Testing bad sink with NULL pointer...\n");
    CWE476_NULL_Pointer_Dereference__wchar_t_52c_badSink(nullData); // should cause a problem

    // Test goodG2B with valid data
    printf("Testing goodG2B sink with valid data...\n");
    CWE476_NULL_Pointer_Dereference__wchar_t_52c_goodG2BSink(validData); // should work fine

    // Test goodB2G with NULL pointer
    printf("Testing goodB2G sink with NULL pointer...\n");
    CWE476_NULL_Pointer_Dereference__wchar_t_52c_goodB2GSink(nullData); // should print "data is NULL"

    // Test goodB2G with valid data
    printf("Testing goodB2G sink with valid data...\n");
    CWE476_NULL_Pointer_Dereference__wchar_t_52c_goodB2GSink(validData); // should work fine

    return 0;
}