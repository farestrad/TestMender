#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

typedef struct _CWE122_Heap_Based_Buffer_Overflow__c_src_wchar_t_cat_67_structType
{
    wchar_t * structFirst;
} CWE122_Heap_Based_Buffer_Overflow__c_src_wchar_t_cat_67_structType;

void CWE122_Heap_Based_Buffer_Overflow__c_src_wchar_t_cat_67b_badSink(CWE122_Heap_Based_Buffer_Overflow__c_src_wchar_t_cat_67_structType myStruct)
{
    wchar_t * data = myStruct.structFirst;
    {
        wchar_t dest[50] = L"";
        wcscat(dest, data);
        wprintf(L"Dest after concatenation: %ls\n", dest);
        free(data);
    }
}

void CWE122_Heap_Based_Buffer_Overflow__c_src_wchar_t_cat_67b_goodG2BSink(CWE122_Heap_Based_Buffer_Overflow__c_src_wchar_t_cat_67_structType myStruct)
{
    wchar_t * data = myStruct.structFirst;
    {
        wchar_t dest[50] = L"";
        wcscat(dest, data);
        wprintf(L"Dest after concatenation: %ls\n", dest);
        free(data);
    }
}

void testBadCase()
{
    CWE122_Heap_Based_Buffer_Overflow__c_src_wchar_t_cat_67_structType myStruct;
    myStruct.structFirst = (wchar_t *)malloc(100 * sizeof(wchar_t));
    wmemset(myStruct.structFirst, L'A', 99);
    myStruct.structFirst[99] = L'\0';
    
    CWE122_Heap_Based_Buffer_Overflow__c_src_wchar_t_cat_67b_badSink(myStruct);
}

void testGoodCase()
{
    CWE122_Heap_Based_Buffer_Overflow__c_src_wchar_t_cat_67_structType myStruct;
    myStruct.structFirst = (wchar_t *)malloc(20 * sizeof(wchar_t));
    wcscpy(myStruct.structFirst, L"Short string");
    
    CWE122_Heap_Based_Buffer_Overflow__c_src_wchar_t_cat_67b_goodG2BSink(myStruct);
}

int main()
{
    wprintf(L"Running bad test case:\n");
    testBadCase();
    
    wprintf(L"\nRunning good test case:\n");
    testGoodCase();

    return 0;
}