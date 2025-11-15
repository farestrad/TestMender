#include "std_testcase.h"

void CWE835_Infinite_Loop__while_true_01_bad() 
{
    int i = 0;
    
    /* FLAW: Infinite Loop - while(true) with no break point */
    while(1)
    {
        printIntLine(i);
        i++;
        if (i == 5) 
        {
            // Simulating a scenario where we expect the loop to be stopped.
            printLine("Unexpected behavior: Infinite loop happening.");
            break; // This should not be executed in a true infinite loop scenario.
        }
    }
}

static void good1() 
{
    int i = 0;

    while(1)
    {
        /* FIX: Add a break point for the loop if i = 10 */
        if (i == 10) 
        { 
            break; 
        }
        printIntLine(i);
        i++;
    }
}

int main(int argc, char * argv[]) 
{
    /* Test the bad scenario */
    printLine("Calling bad()...");
    CWE835_Infinite_Loop__while_true_01_bad();
    printLine("Finished bad()");

    /* Test the good scenario */
    printLine("Calling good()...");
    good1();
    printLine("Finished good()");

    return 0;
} 