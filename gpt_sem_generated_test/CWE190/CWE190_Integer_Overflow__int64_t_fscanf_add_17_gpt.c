#include <inttypes.h>
#include <stdio.h>
#include <limits.h>

void test_overflow_bad()
{
    int64_t data;
    data = 0LL;

    // Simulating bad input, potentially leading to overflow
    printf("Enter a number: ");
    scanf("%" SCNd64, &data);

    // Potential overflow
    int64_t result = data + 1; // This may overflow
    printf("Bad test result: %" PRId64 "\n", result);
}

void test_overflow_good()
{
    int64_t data;
    data = 0LL;

    // Using a controlled good input value
    data = 2;

    // Safe operation
    if (data < LLONG_MAX) {
        int64_t result = data + 1; // This should not overflow
        printf("Good test result: %" PRId64 "\n", result);
    } else {
        printf("Good test: data value is too large to perform arithmetic safely.\n");
    }
}

int main()
{
    printf("Testing bad case...\n");
    test_overflow_bad();
    
    printf("Testing good case...\n");
    test_overflow_good();

    return 0;
}