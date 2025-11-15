#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void badSink(int dataArray[]) {
    // Potentially out-of-bounds access
    int index = dataArray[2];
    int buffer[10];
    if (index >= 0 && index < 10) {
        buffer[index] = 1; // Safe access for valid index
    } else {
        printf("Buffer overflow detected! Invalid index: %d\n", index);
    }
}

void goodG2BSink(int dataArray[]) {
    // Safe access
    int index = dataArray[2];
    int buffer[10];
    if (index >= 0 && index < 10) {
        buffer[index] = 1; // Safe access for valid index
    } else {
        printf("Buffer overflow detected! Invalid index: %d\n", index);
    }
}

void goodB2GSink(int dataArray[]) {
    // Safe access
    int index = dataArray[2];
    int buffer[10];
    if (index >= 0 && index < 10) {
        buffer[index] = 1; // Safe access for valid index
    } else {
        printf("Buffer overflow detected! Invalid index: %d\n", index);
    }
}

void test_bad() {
    int data;
    int dataArray[5];
    data = 10; // Invalid index for buffer
    dataArray[2] = data; 
    badSink(dataArray);
}

void test_goodG2B() {
    int data;
    int dataArray[5];
    data = 7; // Valid index for buffer
    dataArray[2] = data; 
    goodG2BSink(dataArray);
}

void test_goodB2G() {
    int data;
    int dataArray[5];
    data = 10; // Invalid index for buffer
    dataArray[2] = data; 
    goodB2GSink(dataArray);
}

int main() {
    srand((unsigned)time(NULL));

    printf("Running bad test...\n");
    test_bad();

    printf("Running goodG2B test...\n");
    test_goodG2B();

    printf("Running goodB2G test...\n");
    test_goodB2G();

    return 0;
}