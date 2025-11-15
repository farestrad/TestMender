```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define ALLOCA(size) alloca(size)

static int staticReturnsTrue()
{
    return 1;
}

static int staticReturnsFalse()
{
    return 0;
}

void CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_alloca_memmove_08_bad()
{
    int64_t * data;
    int64_t * dataBadBuffer = (int64_t *)ALLOCA(50*sizeof(int64_t));
    int64_t * dataGoodBuffer = (int64_t *)ALLOCA(100*sizeof(int64_t));
    if(staticReturnsTrue())
    {
        data = dataBadBuffer;
    }
    {
        int64_t source[100] = {0}; 
        memmove(data, source, 100*sizeof(int64_t));
        printf("Bad Test - First element: %" PRId64 "\n", data[0]);
    }
}

void CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_alloca_memmove_08_good()
{
    int64_t * data;
    int64_t * dataGoodBuffer = (int64_t *)ALLOCA(100*sizeof(int64_t));
    if(staticReturnsFalse())
    {
        data = NULL; // This should never happen as staticReturnsFalse always returns 0
    }
    else
    {
        data = dataGoodBuffer;
    }
    {
        int64_t source[100] = {0}; 
        memmove(data, source, 100*sizeof(int64_t));
        printf("Good Test - First element: %" PRId64 "\n", data[0]);
    }
}

int main()
{
    printf("Running bad test...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_alloca_memmove_08_bad();
    
    printf("Running good test...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_alloca_memmove_08_good();

    return 0;
}
```