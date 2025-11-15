#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

// Function to cause a heap-based buffer overflow
void bufferOverflowTest(wchar_t *data) {
    // Intentionally copies more than allocated
    wchar_t src[20] = SRC_STRING;
    memcpy(data, src, sizeof(src));
}

// Harness function for the bad case
void testBadCase() {
    wchar_t *data;
    data = (wchar_t *)malloc(10 * sizeof(wchar_t)); // Insufficient space for null terminator
    if (data == NULL) { exit(-1); }
    bufferOverflowTest(data);
    wprintf(L"Data after overflow: %ls\n", data);
    free(data);
}

// Harness function for the good case
void testGoodCase() {
    wchar_t *data;
    data = (wchar_t *)malloc((10 + 1) * sizeof(wchar_t)); // Allocates space for null terminator
    if (data == NULL) { exit(-1); }
    bufferOverflowTest(data);
    wprintf(L"Data after copy: %ls\n", data);
    free(data);
}

int main() {
    wprintf(L"Testing bad case...\n");
    testBadCase(); // Expected to cause an overflow

    wprintf(L"\nTesting good case...\n");
    testGoodCase(); // Should work fine

    return 0;
}