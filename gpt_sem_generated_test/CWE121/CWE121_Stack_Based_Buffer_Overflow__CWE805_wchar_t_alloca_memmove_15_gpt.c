#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#define ALLOCA(size) alloca(size)

void CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_memmove_15_bad()
{
    wchar_t * data;
    wchar_t * dataBadBuffer = (wchar_t *)ALLOCA(50*sizeof(wchar_t));
    switch(6)
    {
    case 6:
        data = dataBadBuffer;
        data[0] = L'\0'; 
        break;
    }
    {
        wchar_t source[100];
        wmemset(source, L'C', 100-1);
        source[100-1] = L'\0'; 
        memmove(data, source, 100*sizeof(wchar_t));
        data[100-1] = L'\0'; 
        wprintf(L"Bad case output: %ls\n", data);
    }
}

void test_bad_case()
{
    wprintf(L"Testing bad case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_memmove_15_bad();
}

void CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_memmove_15_good()
{
    wchar_t * data;
    wchar_t * dataGoodBuffer = (wchar_t *)ALLOCA(100*sizeof(wchar_t));
    switch(5)
    {
    default:
        data = dataGoodBuffer;
        data[0] = L'\0'; 
        break;
    }
    {
        wchar_t source[100];
        wmemset(source, L'C', 100-1);
        source[100-1] = L'\0'; 
        memmove(data, source, 100*sizeof(wchar_t));
        data[100-1] = L'\0'; 
        wprintf(L"Good case output: %ls\n", data);
    }
}

void test_good_case()
{
    wprintf(L"Testing good case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_memmove_15_good();
}

int main()
{
    test_bad_case();
    test_good_case();
    return 0;
}