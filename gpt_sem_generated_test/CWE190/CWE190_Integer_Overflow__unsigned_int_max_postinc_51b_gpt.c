#include <stdio.h>
#include <limits.h>

void printUnsignedLine(unsigned int num) {
    printf("%u\n", num);
}

void CWE190_Integer_Overflow__unsigned_int_max_postinc_51b_badSink(unsigned int data) {
    data++;
    unsigned int result = data;
    printUnsignedLine(result);
}

void CWE190_Integer_Overflow__unsigned_int_max_postinc_51b_goodG2BSink(unsigned int data) {
    data++;
    unsigned int result = data;
    printUnsignedLine(result);
}

void CWE190_Integer_Overflow__unsigned_int_max_postinc_51b_goodB2GSink(unsigned int data) {
    if (data < UINT_MAX) {
        data++;
        unsigned int result = data;
        printUnsignedLine(result);
    } else {
        printf("data value is too large to perform arithmetic safely.\n");
    }
}

void testBadCase() {
    unsigned int data = UINT_MAX; // potential overflow here
    printf("Testing bad case:\n");
    CWE190_Integer_Overflow__unsigned_int_max_postinc_51b_badSink(data);
}

void testGoodG2BCase() {
    unsigned int data = 2; // safe value
    printf("Testing good G2B case:\n");
    CWE190_Integer_Overflow__unsigned_int_max_postinc_51b_goodG2BSink(data);
}

void testGoodB2GCase() {
    unsigned int data = UINT_MAX; // potential overflow check
    printf("Testing good B2G case:\n");
    CWE190_Integer_Overflow__unsigned_int_max_postinc_51b_goodB2GSink(data);
}

int main() {
    testBadCase();
    testGoodG2BCase();
    testGoodB2GCase();
    return 0;
}