#include <stdio.h>
#include <limits.h>

void CWE190_Integer_Overflow__short_fscanf_square_51b_badSink(short data);
void CWE190_Integer_Overflow__short_fscanf_square_51b_goodB2GSink(short data);

void testCWE190Bad() {
    short data;
    data = 0;
    printf("Enter a short integer value: ");
    // POTENTIAL FLAW: Use a value input from the console
    fscanf(stdin, "%hd", &data);
    CWE190_Integer_Overflow__short_fscanf_square_51b_badSink(data);
}

void testCWE190GoodB2G() {
    short data;
    data = 0;
    printf("Enter a short integer value for safe squaring: ");
    // POTENTIAL FLAW: Use a value input from the console
    fscanf(stdin, "%hd", &data);
    
    // Check for potential overflow before squaring
    if(data > 0 && data <= sqrt(SHRT_MAX)) {
        CWE190_Integer_Overflow__short_fscanf_square_51b_goodB2GSink(data);
    } else {
        printf("Input causes overflow when squared.\n");
    }
}

int main() {
    printf("Running bad test...\n");
    testCWE190Bad();
    
    printf("\nRunning good B2G test...\n");
    testCWE190GoodB2G();
    
    return 0;
}