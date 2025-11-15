#include <stdio.h>
#include <assert.h>

// Function prototypes for the bad and good sinks
void CWE476_NULL_Pointer_Dereference__int_52c_badSink(int * data);
void CWE476_NULL_Pointer_Dereference__int_52c_goodG2BSink(int * data);
void CWE476_NULL_Pointer_Dereference__int_52c_goodB2GSink(int * data);

void CWE476_NULL_Pointer_Dereference__int_52b_badSink(int * data) {
    CWE476_NULL_Pointer_Dereference__int_52c_badSink(data);
}

void CWE476_NULL_Pointer_Dereference__int_52b_goodG2BSink(int * data) {
    CWE476_NULL_Pointer_Dereference__int_52c_goodG2BSink(data);
}

void CWE476_NULL_Pointer_Dereference__int_52b_goodB2GSink(int * data) {
    CWE476_NULL_Pointer_Dereference__int_52c_goodB2GSink(data);
}

// Bad sink function which dereferences NULL pointer
void CWE476_NULL_Pointer_Dereference__int_52c_badSink(int * data) {
    // This will cause a null pointer dereference if data is NULL
    printf("%d\n", *data);
}

// Good sink function which checks for NULL before dereferencing
void CWE476_NULL_Pointer_Dereference__int_52c_goodB2GSink(int * data) {
    // Good practice: Check for NULL before dereferencing
    if (data != NULL) {
        printf("%d\n", *data);
    } else {
        printf("Data is NULL, not dereferencing to prevent crash.\n");
    }
}

// Good source that initializes data
void CWE476_NULL_Pointer_Dereference__int_52c_goodG2BSink(int * data) {
    int value = 42; // Initialized value
    data = &value; // Assign address of value to data
    printf("%d\n", *data);
}

int main() {
    int *testData = NULL;

    // Test for the bad case
    printf("Testing BAD sink with NULL pointer...\n");
    // Should trigger dereference error
    // Uncommenting the next line will cause a crash
    // CWE476_NULL_Pointer_Dereference__int_52b_badSink(testData);

    // Test for the good source and sink
    printf("Testing GOOD source with GOOD sink...\n");
    CWE476_NULL_Pointer_Dereference__int_52b_goodG2BSink(testData);

    // Test for the bad source and good sink
    printf("Testing GOOD sink with BAD source...\n");
    CWE476_NULL_Pointer_Dereference__int_52b_goodB2GSink(testData);

    return 0;
}