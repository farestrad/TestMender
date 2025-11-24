```c
#include "std_testcase.h"
#include <wchar.h>
#include <string.h>
#include <stdlib.h>

#ifndef OMITBAD
void CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_bad_sink(char * data);

void CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_bad(void)
{
    char * data;
    /* Allocate a small buffer deliberately */
    data = (char *)malloc(50 * sizeof(char));
    if (data == NULL) {exit(-1);}
    data[0] = '\0'; /* null terminate */
    /* Create a large source to append, will overflow */
    char * src = (char *)malloc(100 * sizeof(char));
    if (src == NULL) {exit(-1);}
    memset(src, 'A', 99);
    src[99] = '\0';
    /* Use sink function directly */
    strcat(data, src); /* This should overflow for small data buffer */
    /* For completeness, still call the actual bad sink to match flow variant signature 41 (sink called externally) */
    /* But we've already caused overflow above! */
    CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_bad_sink(data);
    free(data);
    free(src);
}
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_goodG2B_sink(char * data);

void CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_good(void)
{
    char * data;
    /* Allocate a large buffer for safe usage */
    data = (char *)malloc(100 * sizeof(char));
    if (data == NULL) {exit(-1);}
    data[0] = '\0'; /* null terminate */
    /* Create a reasonable sized source */
    char * src = (char *)malloc(50 * sizeof(char));
    if (src == NULL) {exit(-1);}
    memset(src, 'A', 49);
    src[49] = '\0';
    strcat(data, src); /* No overflow */
    CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_goodG2B_sink(data);
    free(data);
    free(src);
}
#endif /* OMITGOOD */

void run_bad(void) {
    printLine("Running bad case...");
    CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_bad();
    printLine("Completed bad case.");
}

void run_good(void) {
    printLine("Running good case...");
    CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_good();
    printLine("Completed good case.");
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