#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

void printIntLine(int intToPrint) {
    printf("%d\n", intToPrint);
}

void printLine(const char *line) {
    printf("%s\n", line);
}

static void badSink(int data) {
    // Incrementing data could cause an overflow
    ++data;
    int result = data;
    printIntLine(result);
}

void testBad() {
    int data = INT_MAX; // Setting data to INT_MAX to trigger overflow
    badSink(data);
}

static void goodSink(int data) {
    // Ensure there will not be an overflow before incrementing data
    if (data < INT_MAX) {
        ++data;
        int result = data;
        printIntLine(result);
    } else {
        printLine("data value is too large to perform arithmetic safely.");
    }
}

void testGood() {
    int data = 2; // A small, non-zero number
    goodSink(data);
}

int main() {
    printf("Testing bad case:\n");
    testBad(); // This should lead to overflow
    
    printf("Testing good case:\n");
    testGood(); // This should perform correctly without overflow
    
    return 0;
}