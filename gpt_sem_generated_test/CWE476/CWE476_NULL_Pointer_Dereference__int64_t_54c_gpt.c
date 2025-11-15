#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#ifndef OMITBAD
void CWE476_NULL_Pointer_Dereference__int64_t_54d_badSink(int64_t * data) {
    printf("%lld\n", *data); // Potential NULL dereference
}
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE476_NULL_Pointer_Dereference__int64_t_54d_goodG2BSink(int64_t * data) {
    printf("%lld\n", *data); // Intended usage
}

void CWE476_NULL_Pointer_Dereference__int64_t_54d_goodB2GSink(int64_t * data) {
    if (data != NULL) {
        printf("%lld\n", *data); // Safe usage
    } else {
        printf("Data is NULL, no dereference attempted.\n");
    }
}
#endif /* OMITGOOD */

void testBadCase() {
    int64_t *data = NULL; // Bad source: Set data to NULL
    printf("Testing bad case...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_54d_badSink(data); // Should cause segmentation fault
}

void testGoodCase() {
    int64_t value = 42;
    int64_t *data = &value; // Good source: Initialize data
    printf("Testing good case...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_54d_goodG2BSink(data); // Should print 42
}

void testGoodB2GCase() {
    int64_t *data = NULL; // Bad source but handled
    printf("Testing good B2G case...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_54d_goodB2GSink(data); // Should not crash, prints NULL message
}

int main() {
    testBadCase();       // Expect segmentation fault
    testGoodCase();      // Expect to print 42
    testGoodB2GCase();   // Expect message about NULL
    return 0;
}