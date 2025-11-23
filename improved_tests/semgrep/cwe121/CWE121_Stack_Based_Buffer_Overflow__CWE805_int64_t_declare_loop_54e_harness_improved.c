```c
#include <stdio.h>
#include <stdint.h>

void CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_declare_loop_54e_badSink(int64_t * data);
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_declare_loop_54e_goodG2BSink(int64_t * data);

void run_bad(void) {
    /* BAD: buffer is too small for the sink's loop (which copies 100 elements) */
    int64_t dataBadBuffer[50];
    /* Optionally initialize buffer to make ASan errors clearer */
    for (size_t i = 0; i < 50; ++i)
        dataBadBuffer[i] = 0;
    printf("Running BAD test case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_declare_loop_54e_badSink(dataBadBuffer);
}

void run_good(void) {
    /* GOOD: buffer is large enough for the sink's loop (which copies 100 elements) */
    int64_t dataGoodBuffer[100];
    /* Optionally initialize for clarity */
    for (size_t i = 0; i < 100; ++i)
        dataGoodBuffer[i] = 0;
    printf("Running GOOD test case...\n");
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