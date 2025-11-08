/*
 * simple_buffer_overflow.c
 * 
 * Simple buffer overflow test case
 * This should be detected by all three static analysis tools
 */

#include <stdio.h>
#include <string.h>

void vulnerable_function() {
    char buffer[10];
    char input[100];
    
    // CWE-120: Buffer overflow using gets()
    printf("Enter your name: ");
    gets(buffer);  // DANGEROUS: No bounds checking
    
    printf("Hello, %s!\n", buffer);
    
    // CWE-120: Buffer overflow using strcpy()
    strcpy(buffer, "This string is way too long for the buffer");
    
    printf("Buffer contents: %s\n", buffer);
}

int main() {
    printf("Simple Buffer Overflow Test\n");
    printf("DO NOT RUN - For static analysis only!\n");
    
    // Uncomment to test (will crash):
    // vulnerable_function();
    
    return 0;
}