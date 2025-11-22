// ```c
#include <stdio.h>
#include <wchar.h>

void CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_alloca_memcpy_63b_badSink(wchar_t * * dataPtr);
void CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_alloca_memcpy_63b_goodG2BSink(wchar_t * * dataPtr);

void run_bad(void) {
    wchar_t * data;
    wchar_t largeBuffer[100];
    wmemset(largeBuffer, L'A', 99); // Fill with 'A's
    largeBuffer[99] = L'\0'; // Null terminate
    data = largeBuffer;
    
    printf("Running bad case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_alloca_memcpy_63b_badSink(&data);
}

void run_good(void) {
    wchar_t * data;
    wchar_t smallBuffer[50];
    wmemset(smallBuffer, L'A', 49); // Fill with 'A's
    smallBuffer[49] = L'\0'; // Null terminate
    data = smallBuffer;
    
    printf("Running good case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_alloca_memcpy_63b_goodG2BSink(&data);
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
// ```