#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void CWE835_Infinite_Loop__do_01_bad() 
{
    int i = 0;
    
    /* FLAW: Infinite Loop - do..while() with no break point */
    do
    {
        printf("%d\n", i);
        i = (i + 1) % 256;
    } while(i >= 0);
}

static void good1() 
{
    int i = 0;

    do
    {
        /* FIX: Add a break point for the loop if i = 10 */
        if (i == 10) 
        { 
            break; 
        }
        printf("%d\n", i);
        i = (i + 1) % 256;
    } while (i >= 0);
}

int main(int argc, char * argv[]) 
{
    printf("Testing bad case...\n");
    CWE835_Infinite_Loop__do_01_bad();
    printf("Finished bad case.\n");

    printf("Testing good case...\n");
    good1();
    printf("Finished good case.\n");

    return 0;
}