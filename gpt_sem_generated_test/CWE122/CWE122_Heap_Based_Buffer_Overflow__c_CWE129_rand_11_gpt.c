#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER_SIZE 10

// Function to simulate the globalReturnsTrue() for testing
int globalReturnsTrue() {
    return 1;
}

// Function to simulate the globalReturnsFalse() for testing
int globalReturnsFalse() {
    return 0;
}

// Vulnerable function
void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_rand_11_bad() {
    int data;
    data = -1;
    if(globalReturnsTrue()) {
        data = rand() % 20;  // Random value possibly exceeding buffer size
    }
    if(globalReturnsTrue()) {
        int i;
        int *buffer = (int *)malloc(BUFFER_SIZE * sizeof(int));
        if (buffer == NULL) {
            exit(-1);
        }
        for (i = 0; i < BUFFER_SIZE; i++) {
            buffer[i] = 0;
        }
        if (data >= 0) {
            buffer[data] = 1;  // Potential buffer overflow
            for(i = 0; i < BUFFER_SIZE; i++) {
                printf("%d ", buffer[i]);
            }
            printf("\n");
        } else {
            printf("ERROR: Array index is negative.\n");
        }
        free(buffer);
    }
}

// Good function that prevents buffer overflow
void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_good() {
    int data;
    data = -1;
    if(globalReturnsTrue()) {
        data = rand() % BUFFER_SIZE;  // Random value within the buffer size
    }
    if(globalReturnsTrue()) {
        int i;
        int *buffer = (int *)malloc(BUFFER_SIZE * sizeof(int));
        if (buffer == NULL) {
            exit(-1);
        }
        for (i = 0; i < BUFFER_SIZE; i++) {
            buffer[i] = 0;
        }
        if (data >= 0 && data < BUFFER_SIZE) {
            buffer[data] = 1;  // Valid index
            for(i = 0; i < BUFFER_SIZE; i++) {
                printf("%d ", buffer[i]);
            }
            printf("\n");
        } else {
            printf("ERROR: Array index is out-of-bounds\n");
        }
        free(buffer);
    }
}

int main() {
    srand((unsigned)time(NULL));

    printf("Testing bad function:\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE129_rand_11_bad();

    printf("Testing good function:\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE129_good();

    return 0;
}