#include <stdio.h>
#include <limits.h>
#include <math.h>

void CWE190_Integer_Overflow__short_max_square_16_bad() {
    short data;
    data = SHRT_MAX; // Potentially causing overflow
    short result = data * data; // This can overflow
    printf("Bad Test Result: %d\n", result);
}

void CWE190_Integer_Overflow__short_max_square_16_goodB2G() {
    short data;
    data = SHRT_MAX; // Potentially causing overflow
    if (abs((long)data) <= (long)sqrt((double)SHRT_MAX)) {
        short result = data * data; 
        printf("GoodB2G Test Result: %d\n", result);
    } else {
        printf("GoodB2G: data value is too large to perform arithmetic safely.\n");
    }
}

void CWE190_Integer_Overflow__short_max_square_16_goodG2B() {
    short data;
    data = 2; // Safe small value
    short result = data * data; // This will not overflow
    printf("GoodG2B Test Result: %d\n", result);
}

int main() {
    printf("Running bad test case...\n");
    CWE190_Integer_Overflow__short_max_square_16_bad();
    
    printf("Running goodB2G test case...\n");
    CWE190_Integer_Overflow__short_max_square_16_goodB2G();

    printf("Running goodG2B test case...\n");
    CWE190_Integer_Overflow__short_max_square_16_goodG2B();

    return 0;
}