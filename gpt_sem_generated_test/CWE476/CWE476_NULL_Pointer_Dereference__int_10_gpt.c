#include <stdio.h>
#include <stdlib.h>

int globalTrue = 1;
int globalFalse = 0;

#ifndef OMITBAD

void CWE476_NULL_Pointer_Dereference__int_10_bad()
{
    int * data;
    if(globalTrue)
    {
        data = NULL; // Set data to NULL
    }
    if(globalTrue)
    {
        printf("Attempting to dereference data...\n");
        printf("%d\n", *data); // Attempt to use data, which may be NULL
    }
}

#endif /* OMITBAD */

#ifndef OMITGOOD

void goodB2G1()
{
    int * data;
    if(globalTrue)
    {
        data = NULL; // Set data to NULL
    }
    if(globalFalse)
    {
        printf("Benign, fixed string\n");
    }
    else
    {
        if (data != NULL)
        {
            printf("%d\n", *data);
        }
        else
        {
            printf("data is NULL\n"); // Expected behavior
        }
    }
}

void goodB2G2()
{
    int * data;
    if(globalTrue)
    {
        data = NULL; // Set data to NULL
    }
    if(globalTrue)
    {
        if (data != NULL)
        {
            printf("%d\n", *data);
        }
        else
        {
            printf("data is NULL\n"); // Expected behavior
        }
    }
}

void goodG2B1()
{
    int * data;
    int tmpData = 5;
    if(globalFalse)
    {
        printf("Benign, fixed string\n");
    }
    else
    {
        data = &tmpData; // Initialize data
    }
    if(globalTrue)
    {
        printf("Attempting to dereference data...\n");
        printf("%d\n", *data); // Potential flaw: dereferencing uninitialized
    }
}

void goodG2B2()
{
    int * data;
    int tmpData = 5;
    if(globalTrue)
    {
        data = &tmpData; // Initialize data
    }
    if(globalTrue)
    {
        printf("Attempting to dereference data...\n");
        printf("%d\n", *data); // Potential flaw: dereferencing uninitialized
    }
}

void CWE476_NULL_Pointer_Dereference__int_10_good()
{
    goodB2G1();
    goodB2G2();
    goodG2B1();
    goodG2B2();
}

#endif /* OMITGOOD */

int main(int argc, char * argv[])
{
    // Testing the bad function
    printf("Calling bad()...\n");
    CWE476_NULL_Pointer_Dereference__int_10_bad();
    printf("Finished bad()\n");

    // Testing the good function
    printf("Calling good()...\n");
    CWE476_NULL_Pointer_Dereference__int_10_good();
    printf("Finished good()\n");

    return 0;
}