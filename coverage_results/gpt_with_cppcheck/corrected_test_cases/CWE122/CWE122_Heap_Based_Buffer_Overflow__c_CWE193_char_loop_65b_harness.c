// ```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SRC_STRING "AAAAAAAAAA"

void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_loop_65b_badSink(char * data);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_loop_65b_goodG2BSink(char * data);

void run_bad(void) {
    char *data = (char *)malloc(10 * sizeof(char)); // Allocating insufficient space
    if (data == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    printf("Running BAD Case\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_loop_65b_badSink(data);
}

void run_good(void) {
    char *data = (char *)malloc((10 + 1) * sizeof(char)); // Allocating enough space for null terminator
    if (data == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    printf("Running GOOD Case\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_loop_65b_goodG2BSink(data);
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
// ```