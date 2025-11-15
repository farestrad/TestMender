#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define GOOD_BUFFER_SIZE 10
#define BAD_BUFFER_SIZE 100

void CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_54c_badSink(int * data);
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_54c_goodG2BSink(int * data);

void CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_54b_badSink(int * data)
{
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_54c_badSink(data);
}

void CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_54b_goodG2BSink(int * data)
{
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_54c_goodG2BSink(data);
}

void CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_54c_badSink(int * data)
{
    int dest[BAD_BUFFER_SIZE];
    memmove(dest, data, BAD_BUFFER_SIZE * sizeof(int)); // Potential overflow
    printf("Bad Sink executed.\n");
}

void CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_54c_goodG2BSink(int * data)
{
    int dest[GOOD_BUFFER_SIZE];
    memmove(dest, data, GOOD_BUFFER_SIZE * sizeof(int)); // Safe copy
    printf("Good Sink executed.\n");
}

void test_bad()
{
    int *data = (int *)alloca(BAD_BUFFER_SIZE * sizeof(int)); // Potentially dangerous allocation
    for(int i = 0; i < BAD_BUFFER_SIZE; i++) {
        data[i] = i; // Fill buffer
    }
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_54b_badSink(data);
}

void test_good()
{
    int data[GOOD_BUFFER_SIZE];
    for (int i = 0; i < GOOD_BUFFER_SIZE; i++) {
        data[i] = i; // Fill buffer
    }
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_54b_goodG2BSink(data);
}

int main(int argc, char *argv[])
{
    printf("Running test for bad case...\n");
    test_bad();
    
    printf("Running test for good case...\n");
    test_good();
    
    return 0;
}