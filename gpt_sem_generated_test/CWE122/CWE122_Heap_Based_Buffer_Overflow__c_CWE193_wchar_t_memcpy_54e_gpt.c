#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define SRC_STRING L"AAAAAAAAAA"

void badSink(wchar_t *data) {
    wchar_t source[10 + 1] = SRC_STRING;
    memcpy(data, source, (wcslen(source) + 1) * sizeof(wchar_t));
    wprintf(L"%ls\n", data);
    free(data);
}

void testBad() {
    wchar_t *data = (wchar_t *)malloc(10 * sizeof(wchar_t)); // No space for NULL terminator
    if (data == NULL) {
        exit(-1);
    }
    badSink(data);
}

void goodG2BSink(wchar_t *data) {
    wchar_t source[10 + 1] = SRC_STRING;
    memcpy(data, source, (wcslen(source) + 1) * sizeof(wchar_t));
    wprintf(L"%ls\n", data);
    free(data);
}

void testGood() {
    wchar_t *data = (wchar_t *)malloc((10 + 1) * sizeof(wchar_t)); // Space for NULL terminator
    if (data == NULL) {
        exit(-1);
    }
    goodG2BSink(data);
}

int main() {
    wprintf(L"Testing bad case:\n");
    testBad(); // Invoke bad test
    wprintf(L"Testing good case:\n");
    testGood(); // Invoke good test
    return 0;
}