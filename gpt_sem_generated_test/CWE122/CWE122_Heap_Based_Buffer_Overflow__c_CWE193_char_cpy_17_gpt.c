```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SRC_STRING "AAAAAAAAAA"

void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_cpy_17_bad()
{
    int i;
    char * data;
    data = NULL;

    for(i = 0; i < 1; i++)
    {
        /* FLAW: Did not leave space for a null terminator */
        data = (char *)malloc(10 * sizeof(char));
        if (data == NULL) {exit(-1);}
    }
    
    {
        char source[10 + 1] = SRC_STRING;
        /* POTENTIAL FLAW: data may not have enough space to hold source */
        strcpy(data, source);
        printf("Bad Test Output: %s\n", data);
        free(data);
    }
}

void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_cpy_17_good()
{
    int h;
    char * data;
    data = NULL;

    for(h = 0; h < 1; h++)
    {
        /* FIX: Allocate space for a null terminator */
        data = (char *)malloc((10 + 1) * sizeof(char));
        if (data == NULL) {exit(-1);}
    }
    
    {
        char source[10 + 1] = SRC_STRING;
        /* POTENTIAL FLAW: data may not have enough space to hold source */
        strcpy(data, source);
        printf("Good Test Output: %s\n", data);
        free(data);
    }
}

int main(int argc, char * argv[])
{
    printf("Calling bad()...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_cpy_17_bad();
    printf("Finished bad()\n");

    printf("Calling good()...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_cpy_17_good();
    printf("Finished good()\n");

    return 0;
}
```