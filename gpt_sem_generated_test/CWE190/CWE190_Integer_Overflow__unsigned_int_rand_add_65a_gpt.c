#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void CWE190_Integer_Overflow__unsigned_int_rand_add_65b_badSink(unsigned int data) {
    unsigned int result = data + 1; // Potential overflow
    printf("Result of adding 1 (potential overflow): %u\n", result);
}

void CWE190_Integer_Overflow__unsigned_int_rand_add_65b_goodB2GSink(unsigned int data) {
    if (data < UINT_MAX) {
        unsigned int result = data + 1; // Safe addition
        printf("Result of adding 1 (safe): %u\n", result);
    } else {
        printf("Prevented overflow when adding 1\n");
    }
}

void test_bad() {
    unsigned int data = (unsigned int)rand();
    printf("Testing bad case with data: %u\n", data);
    CWE190_Integer_Overflow__unsigned_int_rand_add_65b_badSink(data);
}

void test_goodB2G() {
    unsigned int data = (unsigned int)rand();
    printf("Testing goodB2G case with data: %u\n", data);
    CWE190_Integer_Overflow__unsigned_int_rand_add_65b_goodB2GSink(data);
}

int main() {
    srand((unsigned)time(NULL));

    // Test the bad case
    printf("Running bad test:\n");
    test_bad();

    // Test the good case (prevention of overflow)
    printf("Running goodB2G test:\n");
    test_goodB2G();

    return 0;
}