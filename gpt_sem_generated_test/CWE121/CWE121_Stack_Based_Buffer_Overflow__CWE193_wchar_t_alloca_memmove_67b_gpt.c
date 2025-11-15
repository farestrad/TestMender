#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define SRC_STRING L"AAAAAAAAAA"
#define BUFFER_SIZE 10

typedef struct _CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_alloca_memmove_67_structType {
    wchar_t * structFirst;
} CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_alloca_memmove_67_structType;

void CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_alloca_memmove_67b_badSink(CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_alloca_memmove_67_structType myStruct) {
    wchar_t * data = myStruct.structFirst;
    {
        wchar_t source[BUFFER_SIZE + 1] = SRC_STRING;
        memmove(data, source, (wcslen(source) + 1) * sizeof(wchar_t));
        wprintf(L"Copied data: %ls\n", data);
    }
}

void CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_alloca_memmove_67b_goodG2BSink(CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_alloca_memmove_67_structType myStruct) {
    wchar_t * data = myStruct.structFirst;
    {
        wchar_t source[BUFFER_SIZE + 1] = SRC_STRING;
        memmove(data, source, (wcslen(source) + 1) * sizeof(wchar_t));
        wprintf(L"Copied data: %ls\n", data);
    }
}

void testBufferOverflow() {
    wchar_t * data = (wchar_t *)alloca(BUFFER_SIZE * sizeof(wchar_t)); // Insufficient space
    CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_alloca_memmove_67_structType myStruct;
    myStruct.structFirst = data;
    wprintf(L"Testing buffer overflow:\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_alloca_memmove_67b_badSink(myStruct);
}

void testGoodCase() {
    wchar_t * data = (wchar_t *)alloca((BUFFER_SIZE + 1) * sizeof(wchar_t)); // Sufficient space
    CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_alloca_memmove_67_structType myStruct;
    myStruct.structFirst = data;
    wprintf(L"Testing good case:\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_alloca_memmove_67b_goodG2BSink(myStruct);
}

int main() {
    testBufferOverflow();
    testGoodCase();
    return 0;
}