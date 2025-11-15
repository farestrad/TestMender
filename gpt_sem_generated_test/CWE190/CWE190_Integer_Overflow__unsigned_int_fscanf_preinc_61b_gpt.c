#include <stdio.h>
#include <limits.h>
#include <assert.h>

unsigned int CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_61b_badSource(unsigned int data)
{
    fscanf(stdin, "%u", &data);
    return data;
}

unsigned int CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_61b_goodG2BSource(unsigned int data)
{
    data = 2;
    return data;
}

unsigned int CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_61b_goodB2GSource(unsigned int data)
{
    fscanf(stdin, "%u", &data);
    return data;
}

void test_bad_case()
{
    unsigned int data = 0;
    
    printf("Testing bad case...\n");
    data = CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_61b_badSource(data);
    
    // This may cause an overflow if data is UINT_MAX
    unsigned int result = ++data;
    
    if (data == UINT_MAX) {
        printf("Overflow occurred: data is now 0 after incrementing UINT_MAX\n");
    } else {
        printf("No overflow: data = %u\n", data);
    }
}

void test_good_case()
{
    unsigned int data = 0;
    
    printf("Testing good case...\n");
    data = CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_61b_goodG2BSource(data);
    
    // Safe increment
    unsigned int result = ++data;
    
    assert(data == 3); // Expecting 2 + 1 = 3
    printf("No overflow: data = %u\n", data);
}

void test_good_case_b2g()
{
    unsigned int data = 0;
    
    printf("Testing good B2G case...\n");
    data = CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_61b_goodB2GSource(data);
    
    // Check for potential overflow
    if (data < UINT_MAX) {
        unsigned int result = ++data;
        printf("No overflow: data = %u\n", data);
    } else {
        printf("Overflow would occur, operation not performed.\n");
    }
}

int main()
{
    test_bad_case();
    test_good_case();
    test_good_case_b2g();
    return 0;
}