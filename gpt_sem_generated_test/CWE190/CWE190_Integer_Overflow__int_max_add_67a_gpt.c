#include <stdio.h>
#include <limits.h>

// Struct definition for testing
typedef struct _CWE190_Integer_Overflow__int_max_add_67_structType
{
    int structFirst;
} CWE190_Integer_Overflow__int_max_add_67_structType;

// Function prototypes for sinks
void badSink(CWE190_Integer_Overflow__int_max_add_67_structType myStruct);
void goodB2GSink(CWE190_Integer_Overflow__int_max_add_67_structType myStruct);

// Bad Sink Function - Potential Integer Overflow
void badSink(CWE190_Integer_Overflow__int_max_add_67_structType myStruct)
{
    int result = myStruct.structFirst + 1;
    // Check for overflow
    if (myStruct.structFirst == INT_MAX)
    {
        printf("Overflow detected! Result is beyond the limit.\n");
    }
    else
    {
        printf("Result: %d\n", result);
    }
}

// Good Sink Function - Prevents Integer Overflow
void goodB2GSink(CWE190_Integer_Overflow__int_max_add_67_structType myStruct)
{
    if (myStruct.structFirst < INT_MAX)
    {
        int result = myStruct.structFirst + 1;
        printf("Result without overflow: %d\n", result);
    }
    else
    {
        printf("Cannot add 1 to %d due to overflow risk.\n", myStruct.structFirst);
    }
}

// Main Function to run tests
int main()
{
    CWE190_Integer_Overflow__int_max_add_67_structType testData;

    // Test bad case
    printf("Running bad test case...\n");
    testData.structFirst = INT_MAX;
    badSink(testData);

    // Test good case
    printf("Running good test case...\n");
    testData.structFirst = 2; // Use a small, non-zero number
    goodB2GSink(testData);

    return 0;
}