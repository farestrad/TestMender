```c
#include <stdio.h>
#include <stdlib.h>

/* The Juliet testcases define these prototypes */
#ifndef OMITBAD
void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_large_66_bad();
#endif

#ifndef OMITGOOD
void CWE122_Heap_Based_Buffer_Overflow__c_CWE129_large_66_good();
#endif

int main(int argc, char * argv[])
{
    /* If argument count is not 2, print usage */
    if (argc != 2)
    {
        printf("Usage: %s good|bad\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "good") == 0)
    {
#ifndef OMITGOOD
        CWE122_Heap_Based_Buffer_Overflow__c_CWE129_large_66_good();
        printf("GOOD: Completed without crash.\n");
        return 0;
#else
        printf("GOOD path unavailable.\n");
        return 1;
#endif
    }
    else if (strcmp(argv[1], "bad") == 0)
    {
#ifndef OMITBAD
        CWE122_Heap_Based_Buffer_Overflow__c_CWE129_large_66_bad();
        printf("BAD: Completed (should see AddressSanitizer error above if bug triggered).\n");
        return 0;
#else
        printf("BAD path unavailable.\n");
        return 1;
#endif
    }
    else
    {
        printf("Unknown argument '%s'\n", argv[1]);
        printf("Usage: %s good|bad\n", argv[0]);
        return 1;
    }
}
```