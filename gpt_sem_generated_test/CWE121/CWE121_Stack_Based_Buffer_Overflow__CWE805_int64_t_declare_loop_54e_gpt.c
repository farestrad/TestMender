#include "std_testcase.h"

void CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_declare_loop_54e_badSink(int64_t * data);
void CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_declare_loop_54e_goodG2BSink(int64_t * data);

#ifndef OMITBAD
void runBadTest() {
    int64_t data[50]; // Smaller than source size to trigger overflow
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_declare_loop_54e_badSink(data);
    // Test for overflow effect
    if(data[0] != 0) {
        printf("Potential buffer overflow occurred in bad test.\n");
    } else {
        printf("No overflow detected in bad test.\n");
    }
}
#endif /* OMITBAD */

#ifndef OMITGOOD
void runGoodTest() {
    int64_t data[100]; // Correct size to prevent overflow
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int64_t_declare_loop_54e_goodG2BSink(data);
    // Test for no overflow effect
    if(data[0] == 0) {
        printf("No overflow detected in good test.\n");
    } else {
        printf("Unexpected value detected in good test.\n");
    }
}
#endif /* OMITGOOD */

int main() {
#ifndef OMITBAD
    printf("Running bad test...\n");
    runBadTest();
#endif /* OMITBAD */

#ifndef OMITGOOD
    printf("Running good test...\n");
    runGoodTest();
#endif /* OMITGOOD */
    return 0;
}