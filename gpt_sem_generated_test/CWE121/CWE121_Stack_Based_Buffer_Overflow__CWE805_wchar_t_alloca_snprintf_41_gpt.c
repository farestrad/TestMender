#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>

#ifdef _WIN32
#define SNPRINTF _snwprintf
#else
#define SNPRINTF swprintf
#endif

void CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_snprintf_41_badSink(wchar_t * data)
{
    {
        wchar_t source[100];
        wmemset(source, L'C', 100-1); /* fill with L'C's */
        source[100-1] = L'\0'; /* null terminate */
        /* POTENTIAL FLAW: Possible buffer overflow if the size of data is less than the length of source */
        SNPRINTF(data, 100, L"%s", source);
        wprintf(L"%ls\n", data);
    }
}

void CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_snprintf_41_bad()
{
    wchar_t * data;
    wchar_t * dataBadBuffer = (wchar_t *)alloca(50*sizeof(wchar_t));
    /* FLAW: Set a pointer to a "small" buffer. */
    data = dataBadBuffer;
    data[0] = L'\0'; /* null terminate */
    CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_snprintf_41_badSink(data);
}

void CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_snprintf_41_goodG2BSink(wchar_t * data)
{
    {
        wchar_t source[100];
        wmemset(source, L'C', 100-1); /* fill with L'C's */
        source[100-1] = L'\0'; /* null terminate */
        /* POTENTIAL FLAW: Possible buffer overflow if the size of data is less than the length of source */
        SNPRINTF(data, 100, L"%s", source);
        wprintf(L"%ls\n", data);
    }
}

static void goodG2B()
{
    wchar_t * data;
    wchar_t * dataGoodBuffer = (wchar_t *)alloca(100*sizeof(wchar_t));
    /* FIX: Set a pointer to a "large" buffer */
    data = dataGoodBuffer;
    data[0] = L'\0'; /* null terminate */
    CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_snprintf_41_goodG2BSink(data);
}

int main(int argc, char * argv[])
{
    /* Bad Test */
    wprintf(L"Calling bad()...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_alloca_snprintf_41_bad();
    wprintf(L"Finished bad()\n");

    /* Good Test */
    wprintf(L"Calling good()...\n");
    goodG2B();
    wprintf(L"Finished good()\n");

    return 0;
}