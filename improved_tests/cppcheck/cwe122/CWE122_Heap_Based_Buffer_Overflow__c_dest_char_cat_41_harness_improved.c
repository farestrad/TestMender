```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Prototypes for the Juliet entry functions (defined in the Juliet test case sources) */
void CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_badSink(char * data);
void CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_goodG2BSink(char * data);

/* BAD: Set up a small buffer to reliably trigger heap-based overflow */
void run_bad(void) {
    printf("Executing bad function...\n");
    /* Allocate a small buffer (smaller than the large source string used in the sink) */
    size_t badBufferSize = 50; /* Intentionally too small for concatenation */
    char *data = (char *)malloc(badBufferSize * sizeof(char));
    if (data == NULL) { exit(1); }
    data[0] = '\0'; /* Null terminate */
    /* The sink will attempt to strcat a large string (>50 chars) to this buffer, triggering overflow */
    CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_badSink(data);
    free(data);
}

/* GOOD: Set up a sufficiently large buffer to avoid overflow */
void run_good(void) {
    printf("Executing good function...\n");
    size_t goodBufferSize = 100; /* Large enough for the sink's concatenation (typically 100) */
    char *data = (char *)malloc(goodBufferSize * sizeof(char));
    if (data == NULL) { exit(1); }
    data[0] = '\0'; /* Null terminate */
    CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_goodG2BSink(data);
    free(data);
}

/* Entry point; control which path via TEST_MODE_BAD macro */
int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```