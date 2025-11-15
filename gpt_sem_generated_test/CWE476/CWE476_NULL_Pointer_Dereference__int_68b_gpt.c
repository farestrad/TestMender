#include "std_testcase.h"
#include <stdio.h>

int * CWE476_NULL_Pointer_Dereference__int_68_badData;
int * CWE476_NULL_Pointer_Dereference__int_68_goodG2BData;
int * CWE476_NULL_Pointer_Dereference__int_68_goodB2GData;

void CWE476_NULL_Pointer_Dereference__int_68b_badSink();
void CWE476_NULL_Pointer_Dereference__int_68b_goodG2BSink();
void CWE476_NULL_Pointer_Dereference__int_68b_goodB2GSink();

void testBadSink() {
    int x = 42;
    CWE476_NULL_Pointer_Dereference__int_68_badData = NULL; // Set to NULL to trigger bad behavior
    printf("Testing Bad Sink:\n");
    CWE476_NULL_Pointer_Dereference__int_68b_badSink(); // Should produce a Null Pointer Dereference
}

void testGoodG2BSink() {
    int x = 42;
    CWE476_NULL_Pointer_Dereference__int_68_goodG2BData = &x; // Initialized correctly
    printf("Testing Good G2B Sink:\n");
    CWE476_NULL_Pointer_Dereference__int_68b_goodG2BSink(); // Should print 42
}

void testGoodB2GSink() {
    int x = 42;
    CWE476_NULL_Pointer_Dereference__int_68_goodB2GData = NULL; // Set to NULL to test NULL check
    printf("Testing Good B2G Sink:\n");
    CWE476_NULL_Pointer_Dereference__int_68b_goodB2GSink(); // Should print "data is NULL"
}

int main() {
    testBadSink();
    testGoodG2BSink();
    testGoodB2GSink();
    return 0;
}