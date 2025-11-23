```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SRC_STRING "AAAAAAAAAA"

void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_loop_65b_badSink(char * data);
void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_loop_65b_goodG2BSink(char * data);

void run_bad(void) {
    /* BAD: Allocate buffer of size 10 with NO room for NULL terminator */
    char *data = (char *)malloc(10 * sizeof(char));
    if (data == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    /* Simulate uninitialized buffer (as in many Juliet BAD flows) */
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_loop_65b_badSink(data);
    free(data);
}

void run_good(void) {
    /* GOOD: Allocate buffer with room for NULL terminator */
    char *data = (char *)malloc((10 + 1) * sizeof(char));
    if (data == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    /* Ensure buffer is initialized (if required by the sink implementation) */
    /* No copy or fill into data here; sink will fill */
    CWE122_Heap_Based_Buffer_Overflow__c_CWE193_char_loop_65b_goodG2BSink(data);
    free(data);
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