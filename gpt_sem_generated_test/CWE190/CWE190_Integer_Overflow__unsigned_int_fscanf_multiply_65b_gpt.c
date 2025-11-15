#include <stdio.h>
#include <limits.h>

#ifndef OMITBAD

void CWE190_Integer_Overflow__unsigned_int_fscanf_multiply_65b_badSink(unsigned int data)
{
    if(data > 0) /* ensure we won't have an underflow */
    {
        /* POTENTIAL FLAW: if (data*2) > UINT_MAX, this will overflow */
        unsigned int result = data * 2;
        printf("Result: %u\n", result);
    }
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodG2B uses the GoodSource with the BadSink */
void CWE190_Integer_Overflow__unsigned_int_fscanf_multiply_65b_goodG2BSink(unsigned int data)
{
    if(data > 0) /* ensure we won't have an underflow */
    {
        /* POTENTIAL FLAW: if (data*2) > UINT_MAX, this will overflow */
        unsigned int result = data * 2;
        printf("Result: %u\n", result);
    }
}

/* goodB2G uses the BadSource with the GoodSink */
void CWE190_Integer_Overflow__unsigned_int_fscanf_multiply_65b_goodB2GSink(unsigned int data)
{
    if(data > 0) /* ensure we won't have an underflow */
    {
        /* FIX: Add a check to prevent an overflow from occurring */
        if (data < (UINT_MAX/2))
        {
            unsigned int result = data * 2;
            printf("Result: %u\n", result);
        }
        else
        {
            printf("data value is too large to perform arithmetic safely.\n");
        }
    }
}

#endif /* OMITGOOD */

void testBad()
{
    unsigned int data;
    printf("Enter an unsigned integer: ");
    fscanf(stdin, "%u", &data);
    CWE190_Integer_Overflow__unsigned_int_fscanf_multiply_65b_badSink(data);
}

void testGoodG2B()
{
    unsigned int data = 2; // good source
    CWE190_Integer_Overflow__unsigned_int_fscanf_multiply_65b_goodG2BSink(data);
}

void testGoodB2G()
{
    unsigned int data;
    printf("Enter an unsigned integer: ");
    fscanf(stdin, "%u", &data);
    CWE190_Integer_Overflow__unsigned_int_fscanf_multiply_65b_goodB2GSink(data);
}

int main()
{
    printf("Running Bad Test:\n");
    testBad();
    
    printf("\nRunning Good Test (G2B):\n");
    testGoodG2B();
    
    printf("\nRunning Good Test (B2G):\n");
    testGoodB2G();
    
    return 0;
}