#include <stdio.h>
#include <string.h>
#include <assert.h>

#define GLOBAL_CONST_FIVE 5

// Test to trigger bad case
void testBad() {
    char *data;
    char dataBadBuffer[50];
    char source[100];
    memset_s(source, sizeof(source), 'C', 100-1); // fill with 'C's
    source[100-1] = '\0'; // null terminate

    if (GLOBAL_CONST_FIVE == 5) {
        // FLAW: Set a pointer to a "small" buffer.
        data = dataBadBuffer;
        data[0] = '\0'; // null terminate
    }

    // This will cause a buffer overflow
    strncpy(data, source, sizeof(source) - 1);
    data[sizeof(source) - 1] = '\0'; // Ensure null termination

    // Check if overflow occurred
    printf("%s\n", data);
}

// Test to trigger good case
void testGood() {
    char *data;
    char dataGoodBuffer[100];
    char source[100];
    memset_s(source, sizeof(source), 'C', 100-1); // fill with 'C's
    source[100-1] = '\0'; // null terminate

    if (GLOBAL_CONST_FIVE != 5) {
        printf("This code should not run in good case.\n");
    } else {
        // FIX: Set a pointer to a "large" buffer
        data = dataGoodBuffer;
        data[0] = '\0'; // null terminate
    }

    // Copying into the good buffer
    strncpy(data, source, sizeof(source) - 1);
    data[sizeof(source) - 1] = '\0'; // Ensure null termination

    // Check for successful copy
    printf("%s\n", data);
}

int main() {
    printf("Testing bad case:\n");
    testBad();

    printf("Testing good case:\n");
    testGood();

    return 0;
}