#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define SNPRINTF _snprintf
#else
#define SNPRINTF snprintf
#endif

// Prototype for the vulnerable function
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_char_snprintf_64b_badSink(void * dataVoidPtr);

void testBadBufferOverflow()
{
    char * data;
    // Allocate a small buffer
    data = (char *)malloc(50 * sizeof(char)); // Small buffer
    if (data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    
    // Call the sink with a bad buffer
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_char_snprintf_64b_badSink(&data);
    
    // Check for potential buffer overflow
    if (strcmp(data, "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC") == 0) {
        printf("Buffer overflow detected in bad case.\n");
    } else {
        printf("No overflow detected in bad case, unexpected behavior.\n");
    }
}

int main()
{
    testBadBufferOverflow();
    return 0;
}

void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_char_snprintf_64b_badSink(void * dataVoidPtr)
{
    char * * dataPtr = (char * *)dataVoidPtr;
    char * data = (*dataPtr);
    {
        char source[100];
        memset(source, 'C', 100 - 1);
        source[100 - 1] = '\0';
        
        // Vulnerability triggered here
        SNPRINTF(data, 100, "%s", source);
        printf("Data copied: %s\n", data);
        
        // Securely erase the buffer before freeing
        memset(data, 0, 50); // NOTE: Using a standard memset here, could be replaced with memset_s
        free(data);
    }
}