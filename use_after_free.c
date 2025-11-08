/*
 * use_after_free.c
 * 
 * Use-after-free vulnerability test case
 * CWE-416: Use After Free
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
} User;

void vulnerable_use_after_free() {
    User *user = (User*)malloc(sizeof(User));
    
    user->id = 1;
    strcpy(user->name, "John Doe");
    
    printf("User ID: %d, Name: %s\n", user->id, user->name);
    
    // Free the memory
    free(user);
    
    // CWE-416: Use after free - DANGEROUS!
    printf("User ID after free: %d\n", user->id);
    strcpy(user->name, "Jane Doe");  // Writing to freed memory
    
    // CWE-415: Double free - DANGEROUS!
    free(user);
}

void correct_usage() {
    User *user = (User*)malloc(sizeof(User));
    
    if (user == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    user->id = 1;
    strcpy(user->name, "John Doe");
    
    printf("User ID: %d, Name: %s\n", user->id, user->name);
    
    // Free the memory
    free(user);
    
    // Set pointer to NULL to prevent use-after-free
    user = NULL;
    
    // Now any attempt to use will be caught
    if (user != NULL) {
        printf("User ID: %d\n", user->id);
    }
}

int main() {
    printf("Use-After-Free Vulnerability Test\n");
    printf("DO NOT RUN - For static analysis only!\n");
    
    // Uncomment to test (will crash or misbehave):
    // vulnerable_use_after_free();
    
    printf("\nCorrect usage:\n");
    correct_usage();
    
    return 0;
}