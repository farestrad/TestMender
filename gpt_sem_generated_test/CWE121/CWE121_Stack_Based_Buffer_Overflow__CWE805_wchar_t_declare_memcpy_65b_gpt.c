```c
#include <stdio.h>
#include <string.h>
#include <wchar.h>

void CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_declare_memcpy_65b_badSink(wchar_t * data)
{
    wchar_t source[100];
    wmemset(source, L'C', 100-1); /* fill with L'C's */
    source[100-1] = L'\0'; /* null terminate */
    memcpy(data, source, 100 * sizeof(wchar_t)); // This can cause overflow
    data[100-1] = L'\0'; /* Ensure the destination buffer is null terminated */
    wprintf(L"Data: %ls\n", data);
}

void test_stack_based_buffer_overflow()
{
    wchar_t data[50]; // Smaller buffer than 'source'
    CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_declare_memcpy_65b_badSink(data);

    // Check if we have any unexpected behavior (e.g., out of bounds access)
    if (wcslen(data) >= 50) {
        wprintf(L"Buffer overflow detected!\n");
    } else {
        wprintf(L"Data was copied safely!\n");
    }
}

int main()
{
    wprintf(L"Running test for CWE-121 Stack Based Buffer Overflow...\n");
    test_stack_based_buffer_overflow();
    return 0;
}
```