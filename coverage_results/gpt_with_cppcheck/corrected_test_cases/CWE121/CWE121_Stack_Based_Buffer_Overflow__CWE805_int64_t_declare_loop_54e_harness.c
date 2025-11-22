// ```c
#include "std_testcase.h"

/* Function prototypes */
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_declare_loop_54e_badSink(int64_t * data);
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_declare_loop_54e_goodG2BSink(int64_t * data);

void run_bad(void) {
    int64_t data[50]; // smaller buffer to simulate overflow condition
    printf("Running bad case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_declare_loop_54e_badSink(data);
}

void run_good(void) {
    int64_t data[100]; // properly sized buffer
    printf("Running good case...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_declare_loop_54e_goodG2BSink(data);
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