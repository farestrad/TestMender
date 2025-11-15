#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#ifndef OMITBAD
void badSink(int64_t * data) {
    // POTENTIAL FLAW: Attempt to dereference a NULL pointer
    printf("Value: %lld\n", *data);
}

void testBad() {
    int64_t * data = NULL; // Setting data to NULL
    badSink(data);
}
#endif /* OMITBAD */

#ifndef OMITGOOD
void goodG2BSink(int64_t * data) {
    // Similar flaw as in bad sink, but it's expected good input
    printf("Value: %lld\n", *data);
}

void testGoodG2B() {
    int64_t tmpData = 5LL;
    int64_t * data = &tmpData; // Correctly initialized
    goodG2BSink(data);
}

void goodB2GSink(int64_t * data) {
    // CHECK: Validate before dereferencing
    if (data != NULL) {
        printf("Value: %lld\n", *data);
    } else {
        printf("Data is NULL, cannot dereference.\n");
    }
}

void testGoodB2G() {
    int64_t * data = NULL; // Setting data to NULL
    goodB2GSink(data); // Call sink with NULL to check the handling
}
#endif /* OMITGOOD */

int main(int argc, char * argv[]) {
    // seed randomness
    srand((unsigned)time(NULL));
    
#ifndef OMITGOOD
    printf("Running goodG2B test...\n");
    testGoodG2B();
    printf("Running goodB2G test...\n");
    testGoodB2G();
#endif /* OMITGOOD */
    
#ifndef OMITBAD
    printf("Running bad test...\n");
    testBad();
#endif /* OMITBAD */

    return 0;
}