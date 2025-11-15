#include <stdio.h>
#include <string.h>
#include <wchar.h>

void CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_memcpy_11_bad()
{
    wchar_t * data;
    wchar_t dataBuffer[100];
    data = dataBuffer;
    
    // FLAW: Initialize data as a large buffer that is larger than the small buffer used in the sink
    wmemset(data, L'A', 100-1); 
    data[100-1] = L'\0'; 

    wchar_t dest[50] = L"";
    
    // POTENTIAL FLAW: Possible buffer overflow if data is larger than dest
    memcpy(dest, data, wcslen(data) * sizeof(wchar_t));
    dest[50-1] = L'\0'; 
    wprintf(L"Data: %ls\n", data);
}

void test_bad()
{
    printf("Testing bad case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_memcpy_11_bad();
}

int main()
{
    test_bad();
    
    return 0;
}