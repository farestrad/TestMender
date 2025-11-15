#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_badSink(char * data) {
    {
        char source[100];
        memset(source, 'C', 100 - 1); /* fill with 'C's */
        source[100 - 1] = '\0'; /* null terminate */
        /* POTENTIAL FLAW: Possible buffer overflow if source is larger than sizeof(data)-strlen(data) */
        strcat(data, source);
        printf("Bad Test Output: %s\n", data);
        free(data);
    }
}

void CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_bad() {
    char * data;
    data = (char *)malloc(50 * sizeof(char)); /* FLAW: Small buffer */
    if (data == NULL) { exit(-1); }
    data[0] = '\0'; /* null terminate */
    CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_badSink(data);
}

void CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_goodG2BSink(char * data) {
    {
        char source[100];
        memset(source, 'C', 100 - 1); /* fill with 'C's */
        source[100 - 1] = '\0'; /* null terminate */
        /* The following logic assumes data is large enough */
        strcat(data, source);
        printf("Good Test Output: %s\n", data);
        free(data);
    }
}

void goodG2B() {
    char * data;
    data = (char *)malloc(100 * sizeof(char)); /* FIX: Large buffer */
    if (data == NULL) { exit(-1); }
    data[0] = '\0'; /* null terminate */
    CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_goodG2BSink(data);
}

int main(int argc, char * argv[]) {
    printf("Running Bad Test...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_bad();
    printf("Running Good Test...\n");
    goodG2B();
    return 0;
}