```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define CHAR_ARRAY_SIZE (3 * sizeof(data) + 2)

// Function to simulate the overflow condition
void test_integer_overflow(int data) {
    // Check for potential overflow before adding 1
    if (data < INT_MAX) {
        int result = data + 1;
        printf("Result after addition: %d\n", result);
    } else {
        printf("Data value is too large to perform arithmetic safely. \n");
    }
}

// Function to simulate bad input leading to overflow
void test_bad_input() {
    int data;
    char inputBuffer[CHAR_ARRAY_SIZE];
    
    // Simulating a bad input (e.g., from socket), setting here manually for testing
    data = INT_MAX;  // Intentional overflow condition for testing
    snprintf(inputBuffer, CHAR_ARRAY_SIZE, "%d", data);

    // Convert to int
    data = atoi(inputBuffer);
    printf("Simulating bad input: %d\n", data);
    test_integer_overflow(data);
}

// Function to simulate good input avoiding overflow
void test_good_input() {
    int data;
    
    // Good input that won't cause overflow
    data = 2;  // A small, non-zero safe value
    printf("Simulating good input: %d\n", data);
    test_integer_overflow(data);
}

// Main function to run tests
int main() {
    printf("Testing bad input (potential overflow):\n");
    test_bad_input();  // should indicate an overflow potential

    printf("\nTesting good input (safe addition):\n");
    test_good_input();  // should safely add

    return 0;
}
```