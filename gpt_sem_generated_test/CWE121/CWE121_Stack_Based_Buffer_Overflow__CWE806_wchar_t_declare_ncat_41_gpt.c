#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <time.h>

void CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_ncat_41_badSink(wchar_t * data)
{
    wchar_t dest[50] = L"";
    wcsncat(dest, data, wcslen(data));
    dest[50-1] = L'\0'; 
    wprintf(L"Data sent to sink: %ls\n", data);
}

void CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_ncat_41_bad()
{
    wchar_t * data;
    wchar_t dataBuffer[100];
    data = dataBuffer;
    wmemset(data, L'A', 100-1);
    data[100-1] = L'\0';
    CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_ncat_41_badSink(data);
}

void CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_ncat_41_goodG2BSink(wchar_t * data)
{
    wchar_t dest[50] = L"";
    wcsncat(dest, data, wcslen(data));
    dest[50-1] = L'\0'; 
    wprintf(L"Data sent to sink (Good): %ls\n", data);
}

static void goodG2B()
{
    wchar_t * data;
    wchar_t dataBuffer[100];
    data = dataBuffer;
    wmemset(data, L'A', 50-1);
    data[50-1] = L'\0';
    CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_ncat_41_goodG2BSink(data);
}

int main(int argc, char * argv[])
{
    srand((unsigned)time(NULL));

    wprintf(L"Calling good()...\n");
    goodG2B();
    wprintf(L"Finished good()\n");

    wprintf(L"Calling bad()...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_ncat_41_bad();
    wprintf(L"Finished bad()\n");

    return 0;
}