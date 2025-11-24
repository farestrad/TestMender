```c
#include <stdio.h>

#ifndef OMITBAD
void CWE476_NULL_Pointer_Dereference__int_53d_badSink(int * data);
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE476_NULL_Pointer_Dereference__int_53d_goodG2BSink(int * data);
void CWE476_NULL_Pointer_Dereference__int_53d_goodB2GSink(int * data);
#endif /* OMITGOOD */

void run_bad(void) {
    int *data = NULL; // Bad source: Set data to NULL
    CWE476_NULL_Pointer_Dereference__int_53d_badSink(data);
}

void run_good(void) {
    int value = 12345;
    int *data;

    /* GoodG2B: data is a valid pointer */
    data = &value;
    CWE476_NULL_Pointer_Dereference__int_53d_goodG2BSink(data);

    /* GoodB2G: data is NULL, but sink should handle it safely */
    data = NULL;
    CWE476_NULL_Pointer_Dereference__int_53d_goodB2GSink(data);
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```