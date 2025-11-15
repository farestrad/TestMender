#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

void test_integer_overflow() {
    unsigned int data;
    
    // Test case with random large value to trigger overflow
    data = (unsigned int)RAND32();
    printf("Testing with random data: %u\n", data);

    unsigned int result = data + 1;

    // Check for overflow
    if (data == UINT_MAX) {
        printf("Overflow detected when adding 1 to %u\n", data);
    } else {
        printf("Result of addition is: %u\n", result);
    }
}

void test_integer_overflow_safe_input() {
    unsigned int data = 2; // Safe value
    printf("Testing with safe input: %u\n", data);
    
    unsigned int result = data + 1;

    // Check for overflow
    if (data == UINT_MAX) {
        printf("Overflow detected when adding 1 to %u\n", data);
    } else {
        printf("Result of addition is: %u\n", result);
    }
}

int main() {
    // Seed randomness
    srand((unsigned)time(NULL));

    // Run tests
    test_integer_overflow();
    test_integer_overflow_safe_input();

    return 0;
}