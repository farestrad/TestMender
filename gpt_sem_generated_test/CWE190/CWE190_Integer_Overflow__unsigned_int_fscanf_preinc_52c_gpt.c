#include <stdio.h>
#include <limits.h>

#ifndef OMITBAD
void CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_52c_badSink(unsigned int data)
{
    ++data;
    unsigned int result = data;
    printf("Bad Sink Result: %u\n", result);
}
#endif /* OMITBAD */

#ifndef OMITGOOD

void CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_52c_goodB2GSink(unsigned int data)
{
    if (data < UINT_MAX)
    {
        ++data;
        unsigned int result = data;
        printf("Good Sink Result: %u\n", result);
    }
    else
    {
        printf("Data value is too large to perform arithmetic safely.\n");
    }
}
#endif /* OMITGOOD */

void testBadCase()
{
    unsigned int data;
    printf("Enter a large number (e.g., 4294967295): ");
    fscanf(stdin, "%u", &data);

    CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_52c_badSink(data);
}

void testGoodCase()
{
    unsigned int data = 2; // small non-zero number
    CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_52c_goodB2GSink(data);
}

int main()
{
    printf("Testing bad case...\n");
    testBadCase();

    printf("Testing good case...\n");
    testGoodCase();

    return 0;
}