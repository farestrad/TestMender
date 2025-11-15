#include <stdio.h>
#include <string.h>
#include <wchar.h>

#define SRC_STRING L"AAAAAAAAAA"

void CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_declare_memcpy_15_bad()
{
    wchar_t * data;
    wchar_t dataBadBuffer[10];
    switch(6)
    {
        case 6:
            // FLAW: Set data to a buffer that does not leave room for a NULL terminator
            data = dataBadBuffer;
            data[0] = L'\0'; // null terminate
            break;
    }
    {
        wchar_t source[10+1] = SRC_STRING;
        // POTENTIAL FLAW: May not have enough space for source
        memcpy(data, source, (wcslen(source) + 1) * sizeof(wchar_t));
        wprintf(L"%ls\n", data);
    }
}

void CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_declare_memcpy_15_good()
{
    wchar_t * data;
    wchar_t dataGoodBuffer[10+1];
    switch(5)
    {
        case 5:
            // FIX: Set data to a buffer that leaves room for a NULL terminator
            data = dataGoodBuffer;
            data[0] = L'\0'; // null terminate
            break;
    }
    {
        wchar_t source[10+1] = SRC_STRING;
        // Safe copy since data has space for the null terminator
        memcpy(data, source, (wcslen(source) + 1) * sizeof(wchar_t));
        wprintf(L"%ls\n", data);
    }
}

int main()
{
    printf("Calling bad()...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_declare_memcpy_15_bad();
    printf("Finished bad()\n");

    printf("Calling good()...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_declare_memcpy_15_good();
    printf("Finished good()\n");

    return 0;
}