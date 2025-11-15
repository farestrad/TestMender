#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <assert.h>

#define SRC_STRING L"AAAAAAAAAA"

static const int STATIC_CONST_FIVE = 5;

void CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_declare_cpy_06_bad() {
    wchar_t *data;
    wchar_t dataBadBuffer[10];
    if (STATIC_CONST_FIVE == 5) {
        data = dataBadBuffer;
        data[0] = L'\0'; // null terminate
    }
    wchar_t source[10 + 1] = SRC_STRING;
    wcscpy(data, source); // Potential overflow
    wprintf(L"Resulting string: %ls\n", data);
}

void CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_declare_cpy_06_good() {
    wchar_t *data;
    wchar_t dataGoodBuffer[10 + 1];
    if (STATIC_CONST_FIVE == 5) {
        data = dataGoodBuffer;
        data[0] = L'\0'; // null terminate
    }
    wchar_t source[10 + 1] = SRC_STRING;
    wcscpy(data, source); // Safe copy
    wprintf(L"Resulting string: %ls\n", data);
}

int main() {
    // Bad test case
    wprintf(L"Running bad test case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_declare_cpy_06_bad();
    
    // Good test case
    wprintf(L"Running good test case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_declare_cpy_06_good();

    return 0;
}