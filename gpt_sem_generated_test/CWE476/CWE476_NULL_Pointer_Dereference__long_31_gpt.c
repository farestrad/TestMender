#include "std_testcase.h"
#include <stdio.h>

#ifndef OMITBAD

void CWE476_NULL_Pointer_Dereference__long_31_bad()
{
    long * data;
    /* POTENTIAL FLAW: Set data to NULL */
    data = NULL;
    {
        long * dataCopy = data;
        long * data = dataCopy;
        /* POTENTIAL FLAW: Attempt to use data, which may be NULL */
        printLongLine(*data);
    }
    printf("Executed bad test case.\n");
}

#endif /* OMITBAD */

#ifndef OMITGOOD

static void goodG2B()
{
    long * data;
    long tmpData = 5L;
    /* FIX: Initialize data */
    {
        data = &tmpData;
    }
    {
        long * dataCopy = data;
        long * data = dataCopy;
        /* POTENTIAL FLAW: Attempt to use data, which may be NULL */
        printLongLine(*data);
    }
    printf("Executed goodG2B test case.\n");
}

static void goodB2G()
{
    long * data;
    /* POTENTIAL FLAW: Set data to NULL */
    data = NULL;
    {
        long * dataCopy = data;
        long * data = dataCopy;
        /* FIX: Check for NULL before attempting to print data */
        if (data != NULL)
        {
            printLongLine(*data);
        }
        else
        {
            printLine("data is NULL");
        }
    }
    printf("Executed goodB2G test case.\n");
}

void CWE476_NULL_Pointer_Dereference__long_31_good()
{
    goodG2B();
    goodB2G();
}

#endif /* OMITGOOD */

int main(int argc, char * argv[])
{
    /* seed randomness */
    srand((unsigned)time(NULL));
#ifndef OMITGOOD
    printLine("Calling good()...");
    CWE476_NULL_Pointer_Dereference__long_31_good();
    printLine("Finished good()");
#endif /* OMITGOOD */
#ifndef OMITBAD
    printLine("Calling bad()...");
    CWE476_NULL_Pointer_Dereference__long_31_bad();
    printLine("Finished bad()");
#endif /* OMITBAD */
    return 0;
}