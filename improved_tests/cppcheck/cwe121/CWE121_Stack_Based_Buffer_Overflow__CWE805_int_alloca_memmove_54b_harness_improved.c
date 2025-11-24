
#include <stdio.h>
#include <stdlib.h>

/* Function declarations from Juliet test case */
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_54c_badSink(int * data);
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_54c_goodG2BSink(int * data);

void run_bad(void) {
    /* BAD: Allocate a small buffer, which WILL overflow in the sink */
    int smallBuffer[10];
    int *data = smallBuffer;

    printf("Running bad case\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_54c_badSink(data);
}

void run_good(void) {
    /* GOOD: Allocate a sufficiently large buffer, preventing overflow */
    int largeBuffer[20];
    int *data = largeBuffer;

    printf("Running good case\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_54c_goodG2BSink(data);

    /* Confirm that the buffer is intact and the run was successful (optional for a clean return) */
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}