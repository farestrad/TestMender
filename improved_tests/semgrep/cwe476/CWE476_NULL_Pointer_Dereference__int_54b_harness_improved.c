```c
#include <stdio.h>

/* Declarations for the action functions in the Juliet test */
void CWE476_NULL_Pointer_Dereference__int_54b_badSink(int * data);
void CWE476_NULL_Pointer_Dereference__int_54b_goodG2BSink(int * data);
void CWE476_NULL_Pointer_Dereference__int_54b_goodB2GSink(int * data);

/* 
 * BAD: data is NULL, will be dereferenced in the BAD sink
 */
void run_bad(void) {
    int *data = NULL; // Bad: data is NULL
    printf("Running bad case...\n");
    CWE476_NULL_Pointer_Dereference__int_54b_badSink(data); // Should trigger ASan/null deref
}

/* 
 * GOOD: Good Source - initialize data to point to a valid int
 */
void run_good(void) {
    int safe_val = 123;
    int *data = &safe_val; // Good: data points to valid int
    printf("Running good case (Good Source with Bad Sink)...\n");
    CWE476_NULL_Pointer_Dereference__int_54b_goodG2BSink(data); // Should be safe
}

/* 
 * GOOD: Bad Source (NULL), but sink checks for NULL before use
 */
void run_good_b2g(void) {
    int *data = NULL; // Bad source
    printf("Running good case (Bad Source with Good Sink)...\n");
    CWE476_NULL_Pointer_Dereference__int_54b_goodB2GSink(data); // Should avoid NULL deref
}

/* Main function: runs either BAD or both GOOD cases */
int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
    run_good_b2g();
#endif
    return 0;
}
```