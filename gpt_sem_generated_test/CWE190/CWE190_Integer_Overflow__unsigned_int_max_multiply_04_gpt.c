#include <stdio.h>
#include <limits.h>

void printUnsignedLine(unsigned int data) {
    printf("%u\n", data);
}

void printLine(const char *message) {
    printf("%s\n", message);
}

void test_integer_overflow() {
    unsigned int data;

    // Test bad case: maximum value leading to overflow
    data = UINT_MAX;
    if (data > 0) {
        unsigned int result = data * 2;
        if (result < data) {
            printLine("Overflow occurred in bad test case!");
        } else {
            printUnsignedLine(result);
        }
    }

    // Test good case: value that will not cause overflow
    data = 2;
    if (data > 0) {
        unsigned int result = data * 2;
        if (result < data) {
            printLine("Overflow occurred in good test case!");
        } else {
            printUnsignedLine(result);
        }
    }

    // Test edge case: value just below the maximum to avoid overflow
    data = UINT_MAX / 2;
    if (data > 0) {
        unsigned int result = data * 2;
        if (result < data) {
            printLine("Overflow occurred in edge test case!");
        } else {
            printUnsignedLine(result);
        }
    }

    // Test edge case: maximum value again
    data = UINT_MAX;
    if (data > 0) {
        unsigned int result = data * 2;
        if (result < data) {
            printLine("Overflow detected in edge case with maximum value!");
        } else {
            printUnsignedLine(result);
        }
    }
}

int main() {
    test_integer_overflow();
    return 0;
}