#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 100
#define SMALL_BUFFER_SIZE 50

void test_stack_based_buffer_overflow() {
    char *data;
    char dataBadBuffer[SMALL_BUFFER_SIZE];
    char dataGoodBuffer[BUFFER_SIZE];
    
    // Set a pointer to a "small" buffer. This will lead to a possible buffer overflow.
    data = dataBadBuffer;
    data[0] = '\0'; /* null terminate */
    
    char source[BUFFER_SIZE];
    memset(source, 'C', BUFFER_SIZE - 1); /* fill with 'C's */
    source[BUFFER_SIZE - 1] = '\0'; /* null terminate */

    // Potentially unsafe operation that can lead to buffer overflow
    strncat(data, source, BUFFER_SIZE);

    // Output the result
    printf("Result after strncat: %s\n", data);
    if (strlen(data) > SMALL_BUFFER_SIZE - 1) {
        printf("Buffer Overflow Detected!\n");
    }
}

void test_good_case() {
    char *data;
    char dataGoodBuffer[BUFFER_SIZE];
    
    // Set a pointer to a "large" buffer to avoid overflow
    data = dataGoodBuffer;
    data[0] = '\0'; /* null terminate */
    
    char source[BUFFER_SIZE];
    memset(source, 'C', BUFFER_SIZE - 1); /* fill with 'C's */
    source[BUFFER_SIZE - 1] = '\0'; /* null terminate */

    // Safe operation since data points to a larger buffer
    strncat(data, source, BUFFER_SIZE);

    // Outputting the result
    printf("Result after strncat in good case: %s\n", data);
    if (strlen(data) < BUFFER_SIZE) {
        printf("No Buffer Overflow Detected\n");
    }
}

int main() {
    printf("Testing vulnerable case:\n");
    test_stack_based_buffer_overflow();
    
    printf("Testing safe case:\n");
    test_good_case();
    
    return 0;
}