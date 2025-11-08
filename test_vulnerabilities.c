/*
 * test_vulnerabilities.c
 * 
 * Test file containing known security vulnerabilities
 * for testing static analysis tools (Semgrep, Cppcheck, Flawfinder)
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// CWE-120: Buffer Overflow - using gets()
void test_buffer_overflow_gets() {
    char buffer[10];
    printf("Enter input: ");
    gets(buffer);  // UNSAFE: gets() doesn't check buffer bounds
    printf("You entered: %s\n", buffer);
}

// CWE-120: Buffer Overflow - using strcpy()
void test_buffer_overflow_strcpy() {
    char dest[10];
    char src[20] = "This is a very long string";
    strcpy(dest, src);  // UNSAFE: no bounds checking
    printf("Copied: %s\n", dest);
}

// CWE-125: Out-of-bounds Read
void test_out_of_bounds_read() {
    int array[5] = {1, 2, 3, 4, 5};
    int index = 10;
    int value = array[index];  // UNSAFE: reading beyond array bounds
    printf("Value: %d\n", value);
}

// CWE-787: Out-of-bounds Write
void test_out_of_bounds_write() {
    int array[5] = {0};
    int index = 10;
    array[index] = 42;  // UNSAFE: writing beyond array bounds
    printf("Written value\n");
}

// CWE-676: Use of Potentially Dangerous Function
void test_dangerous_scanf() {
    char buffer[10];
    scanf("%s", buffer);  // UNSAFE: no width specifier
    printf("Input: %s\n", buffer);
}

// CWE-134: Uncontrolled Format String
void test_format_string(char *user_input) {
    printf(user_input);  // UNSAFE: format string vulnerability
}

// CWE-457: Use of Uninitialized Variable
void test_uninitialized_variable() {
    int x;
    int result = x + 10;  // UNSAFE: x is not initialized
    printf("Result: %d\n", result);
}

// CWE-401: Memory Leak
void test_memory_leak() {
    char *ptr = (char*)malloc(100);
    printf("Allocated memory\n");
    // UNSAFE: memory not freed
}

// CWE-415: Double Free
void test_double_free() {
    char *ptr = (char*)malloc(100);
    free(ptr);
    free(ptr);  // UNSAFE: double free
}

// CWE-416: Use After Free
void test_use_after_free() {
    char *ptr = (char*)malloc(100);
    strcpy(ptr, "Hello");
    free(ptr);
    printf("%s\n", ptr);  // UNSAFE: use after free
}

// CWE-190: Integer Overflow
void test_integer_overflow() {
    int max = 2147483647;
    int overflow = max + 1;  // UNSAFE: integer overflow
    printf("Overflow: %d\n", overflow);
}

// CWE-369: Divide by Zero
void test_divide_by_zero(int divisor) {
    int result = 100 / divisor;  // UNSAFE: no check for zero
    printf("Result: %d\n", result);
}

// CWE-476: NULL Pointer Dereference
void test_null_pointer() {
    char *ptr = NULL;
    strcpy(ptr, "test");  // UNSAFE: dereferencing NULL
}

// CWE-242: Use of Inherently Dangerous Function
void test_dangerous_getwd() {
    char buffer[10];
    getwd(buffer);  // UNSAFE: obsolete and dangerous
    printf("CWD: %s\n", buffer);
}

int main() {
    printf("=== Static Analysis Test File ===\n");
    printf("This file contains known vulnerabilities.\n");
    printf("DO NOT EXECUTE - for static analysis only!\n");
    
    // Uncomment to test (will crash/misbehave):
    // test_buffer_overflow_gets();
    // test_buffer_overflow_strcpy();
    // test_out_of_bounds_read();
    
    return 0;
}