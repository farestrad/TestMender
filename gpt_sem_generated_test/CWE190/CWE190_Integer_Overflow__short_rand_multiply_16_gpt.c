#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

/* Function to demonstrate potential integer overflow */
void test_integer_overflow_bad() {
    short data;
    data = (short)rand();
    if (data > 0) {
        // Potential overflow
        short result = data * 2;
        printf("Bad Test Result: %d\n", result);
    }
}

/* Function to demonstrate secure handling to prevent overflow */
void test_integer_overflow_good() {
    short data;
    data = (short)(rand() % 100); // Ensure data is small
    if (data > 0) {
        if (data < (SHRT_MAX/2)) {
            short result = data * 2;
            printf("Good Test Result: %d\n", result);
        } else {
            printf("Data value is too large to perform arithmetic safely.\n");
        }
    }
}

int main() {
    srand((unsigned)time(NULL));

    printf("Running Bad Test:\n");
    test_integer_overflow_bad();

    printf("Running Good Test:\n");
    test_integer_overflow_good();

    return 0;
}