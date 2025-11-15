#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <time.h>

static int staticFive = 5;

void CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_declare_ncpy_07_bad() {
    wchar_t * data;
    wchar_t dataBadBuffer[50];
    wchar_t dataGoodBuffer[100];
    if(staticFive==5) {
        data = dataBadBuffer;
        data[0] = L'\0'; /* null terminate */
    }
    {
        wchar_t source[100];
        wmemset(source, L'C', 100-1); /* fill with L'C's */
        source[100-1] = L'\0'; /* null terminate */
        wcsncpy(data, source, 100-1);
        data[100-1] = L'\0'; /* Ensure the destination buffer is null terminated */
        wprintf(L"%ls\n", data);
    }
}

void CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_declare_ncpy_07_good() {
    wchar_t * data;
    wchar_t dataBadBuffer[50];
    wchar_t dataGoodBuffer[100];
    if(staticFive!=5) {
        printLine("Benign, fixed string");
    } else {
        data = dataGoodBuffer;
        data[0] = L'\0'; /* null terminate */
    }
    {
        wchar_t source[100];
        wmemset(source, L'C', 100-1); /* fill with L'C's */
        source[100-1] = L'\0'; /* null terminate */
        wcsncpy(data, source, 100-1);
        data[100-1] = L'\0'; /* Ensure the destination buffer is null terminated */
        wprintf(L"%ls\n", data);
    }
}

int main() {
    srand((unsigned)time(NULL));
    
    wprintf(L"Calling bad()...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_declare_ncpy_07_bad();
    wprintf(L"Finished bad()\n");
    
    wprintf(L"Calling good()...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_declare_ncpy_07_good();
    wprintf(L"Finished good()\n");
    
    return 0;
}