#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 10

// Global variables to control flow
int CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_badGlobal = 1;
int CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B1Global = 0;
int CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B2Global = 1;

int * CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_badSource(int * data)
{
    if(CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_badGlobal)
    {
        // FLAW: Allocate memory without using sizeof(int)
        data = (int *)malloc(BUFFER_SIZE);
        if (data == NULL) {exit(-1);}
    }
    return data;
}

int * CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B1Source(int * data)
{
    if(CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B1Global)
    {
        // No action; should not execute
        printf("Benign, fixed string\n");
    }
    else
    {
        // FIX: Allocate memory using sizeof(int)
        data = (int *)malloc(BUFFER_SIZE * sizeof(int));
        if (data == NULL) {exit(-1);}
    }
    return data;
}

int * CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B2Source(int * data)
{
    if(CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B2Global)
    {
        // FIX: Allocate memory using sizeof(int)
        data = (int *)malloc(BUFFER_SIZE * sizeof(int));
        if (data == NULL) {exit(-1);}
    }
    return data;
}

void testBadSource() {
    int *data = NULL;
    data = CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_badSource(data);
    int buffer[BUFFER_SIZE];

    // Attempt to copy 10 ints, which will overflow the allocated buffer
    memmove(data, buffer, 10 * sizeof(int)); // This is the vulnerability

    printf("Executed testBadSource, potential buffer overflow occurred\n");
    free(data);
}

void testGoodG2B1() {
    int *data = NULL;
    data = CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B1Source(data);
    // No overflow should occur here
    printf("Executed testGoodG2B1, no overflow expected\n");
    free(data);
}

void testGoodG2B2() {
    int *data = NULL;
    data = CWE122_Heap_Based_Buffer_Overflow__CWE131_memmove_22_goodG2B2Source(data);
    // No overflow should occur here
    printf("Executed testGoodG2B2, no overflow expected\n");
    free(data);
}

int main() {
    // Run tests
    testBadSource();
    testGoodG2B1();
    testGoodG2B2();
    return 0;
}