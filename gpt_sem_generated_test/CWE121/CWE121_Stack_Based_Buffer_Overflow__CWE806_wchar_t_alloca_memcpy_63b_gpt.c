#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>

void CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_alloca_memcpy_63b_badSink(wchar_t * * dataPtr);
void CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_alloca_memcpy_63b_goodG2BSink(wchar_t * * dataPtr);

#ifndef OMITBAD

void test_bad()
{
    wchar_t * data;
    wchar_t largeString[100];

    // Initialize data as a large string
    wmemset(largeString, L'A', 99);
    largeString[99] = L'\0';
    data = largeString;
    
    wchar_t * dataPtr = data;
    CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_alloca_memcpy_63b_badSink(&dataPtr);
}

#endif /* OMITBAD */

#ifndef OMITGOOD

void test_good()
{
    wchar_t * data;
    wchar_t smallString[50];

    // Initialize data as a small string
    wcscpy(smallString, L"Short String");
    data = smallString;

    wchar_t * dataPtr = data;
    CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_alloca_memcpy_63b_goodG2BSink(&dataPtr);
}

#endif /* OMITGOOD */

int main(int argc, char * argv[])
{
    printf("Testing bad case...\n");
    test_bad();
    
    printf("Testing good case...\n");
    test_good();

    return 0;
}