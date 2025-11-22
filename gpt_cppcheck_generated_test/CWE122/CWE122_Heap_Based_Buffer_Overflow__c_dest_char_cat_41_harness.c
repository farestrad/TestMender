```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Prototypes for the Juliet entry functions */
void CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_bad(void);
void CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_good(void);

/* BAD: This function executes the bad path */
void run_bad(void) {
    printf("Executing bad function...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_bad();
}

/* GOOD: This function executes the good path */
void run_good(void) {
    printf("Executing good function...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cat_41_good();
}

/* Main function to control the flow based on compile-time flag */
int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```