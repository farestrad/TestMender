// ```c
#include <stdio.h>

void CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_54c_badSink(int * data);
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_54c_goodG2BSink(int * data);

void run_bad(void) {
    // Allocating a small buffer on the stack
    int *data;
    int buffer[10]; // Small buffer
    data = buffer; // Bad: data points to a buffer that may not be large enough
    
    // Simulating a vulnerability by overflowing the buffer
    int overflow_data[20]; // Data that exceeds the buffer size
    for (int i = 0; i < 20; i++) {
        overflow_data[i] = i;
    }

    printf("Running bad case\n");
    // Call to the bad sink that uses memmove, which can cause overflow
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_54c_badSink(data);
}

void run_good(void) {
    // Allocating a sufficient buffer on the stack
    int *data;
    int good_buffer[20]; // Sufficiently large buffer
    data = good_buffer; // Good: data points to a properly sized buffer

    printf("Running good case\n");
    // Call to the good sink which handles the data correctly
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_54c_goodG2BSink(data);
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