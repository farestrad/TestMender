```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void badSink(int data) {
    // Potential overflow
    int result = data + 1;
    if (result < data) {
        printf("Overflow occurred! Result: %d\n", result);
    } else {
        printf("No overflow. Result: %d\n", result);
    }
}

void goodB2GSink(int data) {
    // Good sink: check for overflow before addition
    if (data < INT_MAX) {
        int result = data + 1;
        printf("No overflow. Result: %d\n", result);
    } else {
        printf("Potential overflow, not adding.\n");
    }
}

void testBad() {
    int data = INT_MAX; // setting to max value to cause overflow
    printf("Testing bad scenario...\n");
    badSink(data);
}

void testGood() {
    int data = 2; // setting to a small value to avoid overflow
    printf("Testing good scenario...\n");
    goodB2GSink(data);
}

int main() {
    testBad();
    testGood();
    return 0;
}
```