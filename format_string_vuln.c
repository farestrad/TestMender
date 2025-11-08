/*
 * format_string_vuln.c
 * 
 * Format string vulnerability test case
 * CWE-134: Uncontrolled Format String
 */

#include <stdio.h>
#include <string.h>

void vulnerable_printf(char *user_input) {
    // CWE-134: Format string vulnerability
    printf(user_input);  // DANGEROUS: user input as format string
}

void secure_printf(char *user_input) {
    // SAFE: proper format string usage
    printf("%s", user_input);
}

int main(int argc, char *argv[]) {
    char buffer[100];
    
    printf("Format String Vulnerability Test\n");
    
    if (argc > 1) {
        // Vulnerable - user can inject format specifiers
        printf("Vulnerable version:\n");
        vulnerable_printf(argv[1]);
        
        printf("\n\nSecure version:\n");
        secure_printf(argv[1]);
    } else {
        printf("Usage: %s <input_string>\n", argv[0]);
        printf("Try: %s \"%%x %%x %%x %%x %%s\"\n", argv[0]);
    }
    
    return 0;
}