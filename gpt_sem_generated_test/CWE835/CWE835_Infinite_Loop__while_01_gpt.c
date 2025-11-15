#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef OMITBAD

void CWE835_Infinite_Loop__while_01_bad() 
{
    int i = 0;
    
    /* FLAW: Infinite Loop - while() with no break point */
    while(i >= 0)
    {
        printf("%d\n", i);
        i = (i + 1) % 256;
    }
}

#endif /* OMITBAD */

#ifndef OMITGOOD

static void good1() 
{
    int i = 0;

    while(i >= 0)
    {
        /* FIX: Add a break point for the loop if i = 10 */
        if (i == 10) 
        { 
            break; 
        }
        printf("%d\n", i);
        i = (i + 1) % 256;
    }
}

void CWE835_Infinite_Loop__while_01_good() 
{
    good1();
}

#endif /* OMITGOOD */

int main(int argc, char * argv[]) 
{
    /* seed randomness */
    srand((unsigned)time(NULL));

#ifndef OMITGOOD
    printf("Calling good()...\n");
    CWE835_Infinite_Loop__while_01_good();
    printf("Finished good()\n");
#endif /* OMITGOOD */

#ifndef OMITBAD
    printf("Calling bad()...\n");
    CWE835_Infinite_Loop__while_01_bad();
    printf("Finished bad()\n");
#endif /* OMITBAD */

    return 0;
} 