#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define LLONG_MAX 9223372036854775807LL

int globalFive = 5;

void printLongLongLine(int64_t value)
{
    printf("%lld\n", value);
}

void printLine(const char *str)
{
    printf("%s\n", str);
}

void test_integer_overflow()
{
    int64_t data;
    data = 0LL;

    // Test bad case: Random value that can lead to overflow
    if(globalFive == 5)
    {
        data = (int64_t)rand();  // Use rand() for the bad case
    }
    if(globalFive == 5)
    {
        int64_t result = data + 1;  // Potential overflow here
        printLongLongLine(result);
    }

    // Test good case: With proper check to prevent overflow
    data = 0LL;
    if(globalFive == 5)
    {
        data = (int64_t)rand();  // Use rand() for the bad case
    }
    if(globalFive == 5)
    {
        if (data < LLONG_MAX)
        {
            int64_t result = data + 1;  // No overflow should occur here
            printLongLongLine(result);
        }
        else
        {
            printLine("data value is too large to perform arithmetic safely.");
        }
    }

    // Test good source with potential bad sink
    data = 2;  // Using a small non-zero value
    if(globalFive == 5)
    {
        int64_t result = data + 1;  // This should not overflow
        printLongLongLine(result);
    }
}

int main()
{
    srand((unsigned)time(NULL));  // Seed the random number generator
    printLine("Starting integer overflow tests...");
    test_integer_overflow();
    printLine("Finished integer overflow tests.");
    return 0;
}