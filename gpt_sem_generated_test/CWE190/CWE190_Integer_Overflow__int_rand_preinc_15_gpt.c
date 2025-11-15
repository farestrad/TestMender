#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

void check_integer_overflow(int data) {
    if (data < INT_MAX) {
        ++data;
        printf("Result after increment: %d\n", data);
    } else {
        printf("data value is too large to perform arithmetic safely.\n");
    }
}

void test_overflow() {
    // Test case 1: Random value that might cause overflow
    int data = rand();
    printf("Testing with random data: %d\n", data);
    check_integer_overflow(data);
    
    // Test case 2: Value set close to INT_MAX
    data = INT_MAX - 1;
    printf("Testing with INT_MAX - 1: %d\n", data);
    check_integer_overflow(data);
    
    // Test case 3: Value set to a small integer
    data = 2;
    printf("Testing with small positive value: %d\n", data);
    check_integer_overflow(data);
    
    // Test case 4: Negative value
    data = -1;
    printf("Testing with negative value: %d\n", data);
    check_integer_overflow(data);
}

int main() {
    srand((unsigned)time(NULL)); // Seed randomness
    test_overflow();
    return 0; 
}