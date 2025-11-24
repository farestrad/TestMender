#include "std_testcase.h"
#include <wchar.h>

/* This struct name and field must match the Juliet 67b file */
typedef struct _CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_alloca_memmove_67_structType
{
    wchar_t * structFirst;
} CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_alloca_memmove_67_structType;

#ifndef OMITBAD
void CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_alloca_memmove_67b_badSink(
    CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_alloca_memmove_67_structType myStruct);
#endif

#ifndef OMITGOOD
void CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_alloca_memmove_67b_goodG2BSink(
    CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_alloca_memmove_67_structType myStruct);
#endif

void run_bad(void) {
#ifndef OMITBAD
    /* BAD: buffer too small for SRC_STRING + NULL */
    wchar_t smallBuffer[10];  /* Juliet uses SRC_STRING = L"AAAAAAAAAA" (10 chars) and copies 11 wchar_t */
    CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_alloca_memmove_67_structType myStruct;
    myStruct.structFirst = smallBuffer;

    CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_alloca_memmove_67b_badSink(myStruct);
#endif
}

void run_good(void) {
#ifndef OMITGOOD
    /* GOOD: buffer large enough for SRC_STRING + NULL */
    wchar_t largeBuffer[11];
    CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_alloca_memmove_67_structType myStruct;
    myStruct.structFirst = largeBuffer;

    CWE121_Stack_Based_Buffer_Overflow__CWE193_wchar_t_alloca_memmove_67b_goodG2BSink(myStruct);
#endif
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
