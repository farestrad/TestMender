#include <stdio.h>
#include <stdlib.h>

#ifndef OMITBAD
void CWE476_NULL_Pointer_Dereference__int_54c_badSink(int * data);

void CWE476_NULL_Pointer_Dereference__int_54b_badSink(int * data)
{
    CWE476_NULL_Pointer_Dereference__int_54c_badSink(data);
}

void CWE476_NULL_Pointer_Dereference__int_54c_badSink(int * data)
{
    // Potential NULL pointer dereference
    printf("%d\n", *data);
}
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE476_NULL_Pointer_Dereference__int_54c_goodG2BSink(int * data);

void CWE476_NULL_Pointer_Dereference__int_54b_goodG2BSink(int * data)
{
    CWE476_NULL_Pointer_Dereference__int_54c_goodG2BSink(data);
}

void CWE476_NULL_Pointer_Dereference__int_54c_goodG2BSink(int * data)
{
    // This will dereference safely because the data is initialized
    printf("%d\n", *data);
}

void CWE476_NULL_Pointer_Dereference__int_54c_goodB2GSink(int * data);

void CWE476_NULL_Pointer_Dereference__int_54b_goodB2GSink(int * data)
{
    CWE476_NULL_Pointer_Dereference__int_54c_goodB2GSink(data);
}

void CWE476_NULL_Pointer_Dereference__int_54c_goodB2GSink(int * data)
{
    // Good sink: check for NULL before dereferencing
    if (data != NULL)
    {
        printf("%d\n", *data);
    }
    else
    {
        printf("Data is NULL, cannot dereference!\n");
    }
}
#endif /* OMITGOOD */

int main()
{
    int *ptr = NULL;

    // Test bad case (NULL Dereference)
    printf("Testing bad case (NULL Dereference)\n");
    CWE476_NULL_Pointer_Dereference__int_54b_badSink(ptr);

    // Test good case with initialized data
    int value = 5;
    ptr = &value;
    printf("Testing good case (Initialized Data)\n");
    CWE476_NULL_Pointer_Dereference__int_54b_goodG2BSink(ptr);

    // Test good case with NULL check
    ptr = NULL;
    printf("Testing good case with NULL check\n");
    CWE476_NULL_Pointer_Dereference__int_54b_goodB2GSink(ptr);

    return 0;
}