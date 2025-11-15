#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

static int staticFive = 5;

void CWE122_Heap_Based_Buffer_Overflow__CWE135_07_bad()
{
    void * data;
    data = NULL;
    if(staticFive == 5)
    {
        wchar_t * dataBadBuffer = (wchar_t *)malloc(50 * sizeof(wchar_t));
        if (dataBadBuffer == NULL) {exit(-1);}
        wmemset(dataBadBuffer, L'A', 50 - 1);
        dataBadBuffer[50 - 1] = L'\0';
        data = (void *)dataBadBuffer;
    }
    if(staticFive == 5)
    {
        size_t dataLen = strlen((char *)data); // Potential CWE-122
        void * dest = (void *)calloc(dataLen + 1, sizeof(wchar_t));
        if (dest == NULL) {exit(-1);}
        (void)wcscpy(dest, data);
        printf("%ls\n", (wchar_t *)dest);
        free(dest);
    }
}

void test_bad()
{
    printf("Testing bad case...\n");
    CWE122_Heap_Based_Buffer_Overflow__CWE135_07_bad();
}

void CWE122_Heap_Based_Buffer_Overflow__CWE135_07_good()
{
    void * data;
    data = NULL;
    if(staticFive == 5)
    {
        wchar_t * dataBadBuffer = (wchar_t *)malloc(50 * sizeof(wchar_t));
        if (dataBadBuffer == NULL) {exit(-1);}
        wmemset(dataBadBuffer, L'A', 50 - 1);
        dataBadBuffer[50 - 1] = L'\0';
        data = (void *)dataBadBuffer;
    }
    if(staticFive == 5)
    {
        size_t dataLen = wcslen((wchar_t *)data); // Fixed for wchar_t
        void * dest = (void *)calloc(dataLen + 1, sizeof(wchar_t));
        if (dest == NULL) {exit(-1);}
        (void)wcscpy(dest, data);
        wprintf(L"%ls\n", (wchar_t *)dest);
        free(dest);
    }
}

void test_good()
{
    printf("Testing good case...\n");
    CWE122_Heap_Based_Buffer_Overflow__CWE135_07_good();
}

int main()
{
    test_bad();
    test_good();
    return 0;
}