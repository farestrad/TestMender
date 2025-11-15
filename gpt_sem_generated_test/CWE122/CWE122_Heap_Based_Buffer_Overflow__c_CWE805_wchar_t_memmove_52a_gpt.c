#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

#ifndef OMITBAD
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52b_badSink(wchar_t * data);

void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52_bad()
{
    wchar_t * data;
    data = NULL;
    /* FLAW: Allocate and point data to a small buffer that is smaller than the large buffer used in the sinks */
    data = (wchar_t *)malloc(50 * sizeof(wchar_t));
    if (data == NULL) { exit(-1); }
    data[0] = L'\0'; /* null terminate */
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52b_badSink(data);
}
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52b_goodG2BSink(wchar_t * data);

void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52_good()
{
    wchar_t * data;
    data = NULL;
    /* FIX: Allocate and point data to a large buffer that is at least as large as the large buffer used in the sink */
    data = (wchar_t *)malloc(100 * sizeof(wchar_t));
    if (data == NULL) { exit(-1); }
    data[0] = L'\0'; /* null terminate */
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52b_goodG2BSink(data);
}
#endif /* OMITGOOD */

void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52b_badSink(wchar_t * data)
{
    wchar_t src[100];
    wmemset(src, L'A', 99); // Fill with 'A's
    src[99] = L'\0'; // Null terminate

    // Potential buffer overflow
    memmove(data, src, wcslen(src) * sizeof(wchar_t));
    data[99] = L'\0'; // Make sure data is null terminated
    wprintf(L"Bad case executed with data: %ls\n", data);
}

void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52b_goodG2BSink(wchar_t * data)
{
    wchar_t src[100];
    wmemset(src, L'A', 99); // Fill with 'A's
    src[99] = L'\0'; // Null terminate

    // Safe copy
    memmove(data, src, wcslen(src) * sizeof(wchar_t));
    data[99] = L'\0'; // Make sure data is null terminated
    wprintf(L"Good case executed with data: %ls\n", data);
}

int main(int argc, char * argv[])
{
#ifndef OMITBAD
    wprintf(L"Calling bad()...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52_bad();
    wprintf(L"Finished bad()\n");
#endif /* OMITBAD */
#ifndef OMITGOOD
    wprintf(L"Calling good()...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_wchar_t_memmove_52_good();
    wprintf(L"Finished good()\n");
#endif /* OMITGOOD */
    return 0;
}