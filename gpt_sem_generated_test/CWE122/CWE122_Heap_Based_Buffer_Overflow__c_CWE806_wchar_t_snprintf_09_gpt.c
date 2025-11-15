#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#ifdef _WIN32
#define SNPRINTF _snwprintf
#else
#define SNPRINTF swprintf
#endif

void test_heap_buffer_overflow() {
    wchar_t *data;
    // Allocate buffer on the heap
    data = (wchar_t *)malloc(100 * sizeof(wchar_t));
    if (data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Initialize data as a large buffer, potentially causing overflow
    wmemset(data, L'A', 100 - 1);
    data[100 - 1] = L'\0'; // Null terminate

    wchar_t dest[50] = L"";
    // Attempt to copy data to dest, which is smaller
    SNPRINTF(dest, wcslen(data), L"%s", data);
    
    // Check for overflow by comparing buffer lengths
    if (wcslen(dest) >= sizeof(dest) / sizeof(dest[0])) {
        printf("Buffer overflow detected: dest buffer is too small!\n");
    } else {
        wprintf(L"No overflow: %ls\n", dest);
    }

    free(data);
}

int main() {
    test_heap_buffer_overflow();
    return 0;
}