#include <stdio.h>
#include <wchar.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

void CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_memmove_66b_badSink(wchar_t * dataArray[]);
void CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_memmove_66b_goodG2BSink(wchar_t * dataArray[]);

void CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_memmove_66_bad() {
    wchar_t * data;
    wchar_t * dataArray[5];
    wchar_t dataBuffer[100];
    data = dataBuffer;
    wmemset(data, L'A', 100-1);
    data[100-1] = L'\0';
    dataArray[2] = data;
    CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_memmove_66b_badSink(dataArray);
}

void CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_memmove_66_good() {
    wchar_t * data;
    wchar_t * dataArray[5];
    wchar_t dataBuffer[50]; // Smaller buffer for good case
    data = dataBuffer;
    wmemset(data, L'A', 50-1);
    data[50-1] = L'\0';
    dataArray[2] = data;
    CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_memmove_66b_goodG2BSink(dataArray);
}

int main(int argc, char * argv[]) {
    srand((unsigned)time(NULL));

    printf("Testing bad case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_memmove_66_bad();
    printf("Bad case executed.\n");

    printf("Testing good case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_memmove_66_good();
    printf("Good case executed.\n");

    return 0;
}