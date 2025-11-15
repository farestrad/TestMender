#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define CHAR_ARRAY_SIZE (3 * sizeof(data) + 2)

static void badSink(int data)
{
    int result = data * data;
    printf("Result: %d\n", result);
}

void CWE190_Integer_Overflow__int_fgets_square_41_bad()
{
    int data = 0;
    char inputBuffer[CHAR_ARRAY_SIZE] = "";
    if (fgets(inputBuffer, CHAR_ARRAY_SIZE, stdin) != NULL)
    {
        data = atoi(inputBuffer);
    }
    badSink(data);
}

static void goodG2BSink(int data)
{
    int result = data * data;
    printf("Result: %d\n", result);
}

static void goodG2B()
{
    int data = 2;
    goodG2BSink(data);
}

static void goodB2GSink(int data)
{
    if (data > INT_MIN && abs(data) < (long)sqrt((double)INT_MAX))
    {
        int result = data * data;
        printf("Result: %d\n", result);
    }
    else
    {
        printf("Data value is too large to perform arithmetic safely.\n");
    }
}

static void goodB2G()
{
    int data = 0;
    char inputBuffer[CHAR_ARRAY_SIZE] = "";
    if (fgets(inputBuffer, CHAR_ARRAY_SIZE, stdin) != NULL)
    {
        data = atoi(inputBuffer);
    }
    goodB2GSink(data);
}

int main(int argc, char * argv[])
{
    printf("Testing bad case...\n");
    CWE190_Integer_Overflow__int_fgets_square_41_bad();
    printf("Testing good cases...\n");
    goodB2G();
    goodG2B();
    return 0;
}