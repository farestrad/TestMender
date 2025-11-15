#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* Function prototypes */
void CWE476_NULL_Pointer_Dereference__int64_t_54b_badSink(int64_t * data);
void CWE476_NULL_Pointer_Dereference__int64_t_54b_goodG2BSink(int64_t * data);
void CWE476_NULL_Pointer_Dereference__int64_t_54b_goodB2GSink(int64_t * data);

void CWE476_NULL_Pointer_Dereference__int64_t_54_bad() {
    int64_t * data;
    data = NULL; // Potential flaw: setting data to NULL
    CWE476_NULL_Pointer_Dereference__int64_t_54b_badSink(data);
}

void CWE476_NULL_Pointer_Dereference__int64_t_54_goodG2B() {
    int64_t * data;
    int64_t tmpData = 5LL;
    data = &tmpData; // Proper initialization
    CWE476_NULL_Pointer_Dereference__int64_t_54b_goodG2BSink(data);
}

void CWE476_NULL_Pointer_Dereference__int64_t_54_goodB2G() {
    int64_t * data;
    data = NULL; // Setting to NULL
    CWE476_NULL_Pointer_Dereference__int64_t_54b_goodB2GSink(data);
}

int main() {
    printf("Running bad test...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_54_bad();
    
    printf("Running goodG2B test...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_54_goodG2B();
    
    printf("Running goodB2G test...\n");
    CWE476_NULL_Pointer_Dereference__int64_t_54_goodB2G();
    
    return 0;
}

/* Bad sink demonstration */
void CWE476_NULL_Pointer_Dereference__int64_t_54b_badSink(int64_t * data) {
    printf("Bad Sink: Dereferencing NULL pointer...\n");
    // This will lead to undefined behavior
    printf("%lld\n", *data); // Potential NULL dereference
}

/* GoodG2B sink */
void CWE476_NULL_Pointer_Dereference__int64_t_54b_goodG2BSink(int64_t * data) {
    printf("GoodG2B Sink: Safe dereference.\n");
    printf("%lld\n", *data); // Safe dereference
}

/* GoodB2G sink */
void CWE476_NULL_Pointer_Dereference__int64_t_54b_goodB2GSink(int64_t * data) {
    if (data == NULL) {
        printf("GoodB2G Sink: Data is NULL, not dereferencing.\n");
    } else {
        printf("GoodB2G Sink: Safe dereference.\n");
        printf("%lld\n", *data); // Safe if not NULL
    }
}