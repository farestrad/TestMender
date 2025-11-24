```c
#include "std_testcase.h"
#include <stdio.h>
#include <stdint.h>

/* Function prototypes */
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_declare_loop_54e_badSink(int64_t * data);
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_declare_loop_54e_goodG2BSink(int64_t * data);

void run_bad(void) {
    /* BAD: Buffer is too small and will be overflowed in the sink */
    int64_t dataBadBuffer[50];
    /* Initialize buffer with known values (not strictly necessary, but makes ASan check full access) */
    for (size_t i = 0; i < 50; ++i) {
        dataBadBuffer[i] = 0xBADBADBADBADBADLL;
    }
    printf("Running bad case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_declare_loop_54e_badSink(dataBadBuffer);
}

void run_good(void) {
    /* GOOD: Buffer is appropriately sized and will not be overflowed in the sink */
    int64_t dataGoodBuffer[100];
    /* Initialize buffer with known values */
    for (size_t i = 0; i < 100; ++i) {
        dataGoodBuffer[i] = 0xCAFEBABECAFEBABELL;
    }
    printf("Running good case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_declare_loop_54e_goodG2BSink(dataGoodBuffer);
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```