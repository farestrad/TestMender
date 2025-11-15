#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define CHAR_ARRAY_SIZE (3 * sizeof(data) + 2)

bool globalReturnsTrue() {
    return true;
}

bool globalReturnsFalse() {
    return false;
}

void printIntLine(int data) {
    printf("%d\n", data);
}

void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_fgets_11_test() {
    int data;
    /* Initialize data */
    data = -1;

    if (globalReturnsTrue()) {
        {
            char inputBuffer[CHAR_ARRAY_SIZE] = "";
            printf("Enter an integer (0-9): ");
            /* POTENTIAL FLAW: Read data from the console using fgets() */
            if (fgets(inputBuffer, CHAR_ARRAY_SIZE, stdin) != NULL) {
                /* Convert to int */
                data = atoi(inputBuffer);
            } else {
                printf("fgets() failed.\n");
            }
        }
    }

    {
        int i;
        int *buffer = (int *)malloc(10 * sizeof(int));
        if (buffer == NULL) { exit(-1); }
        /* Initialize buffer */
        for (i = 0; i < 10; i++) {
            buffer[i] = 0;
        }
        
        /* Check for potential buffer overflow */
        if (data >= 0 && data < 10) {
            buffer[data] = 1;
            /* Print the array values */
            for (i = 0; i < 10; i++) {
                printIntLine(buffer[i]);
            }
        } else {
            printf("ERROR: Array index is out-of-bounds\n");
        }
        free(buffer);
    }
}

int main() {
    CWE122_Heap_Based_Buffer_Overflow__c_CWE129_fgets_11_test();
    return 0;
}