#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void printHexCharLine(char value) {
    printf("%02x\n", value);
}

void printLine(const char *line) {
    printf("%s\n", line);
}

void CWE190_Integer_Overflow__char_rand_add_54e_badSink(char data) {
    char result = data + 1; // POTENTIAL FLAW: Adding 1 to data could cause an overflow
    printHexCharLine(result);
}

void CWE190_Integer_Overflow__char_rand_add_54e_goodG2BSink(char data) {
    char result = data + 1; // POTENTIAL FLAW: Adding 1 to data could cause an overflow
    printHexCharLine(result);
}

void CWE190_Integer_Overflow__char_rand_add_54e_goodB2GSink(char data) {
    // FIX: Add a check to prevent an overflow from occurring
    if (data < CHAR_MAX) {
        char result = data + 1;
        printHexCharLine(result);
    } else {
        printLine("data value is too large to perform arithmetic safely.");
    }
}

void test_bad() {
    char data = (char)(rand() % 256); // Simulating bad input
    printf("Testing bad case with data = %d:\n", data);
    CWE190_Integer_Overflow__char_rand_add_54e_badSink(data);
}

void test_goodG2B() {
    char data = 125; // Small, non-zero number
    printf("Testing goodG2B case with data = %d:\n", data);
    CWE190_Integer_Overflow__char_rand_add_54e_goodG2BSink(data);
}

void test_goodB2G() {
    char data = CHAR_MAX; // Simulating risky input
    printf("Testing goodB2G case with data = %d:\n", data);
    CWE190_Integer_Overflow__char_rand_add_54e_goodB2GSink(data);
}

int main() {
    srand((unsigned)time(NULL)); // Seed for randomness

    test_bad();
    test_goodG2B();
    test_goodB2G();

    return 0;
}