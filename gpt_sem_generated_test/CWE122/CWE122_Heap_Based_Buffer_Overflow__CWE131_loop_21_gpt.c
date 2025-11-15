#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printIntLine(int intToPrint) {
    printf("%d\n", intToPrint);
}

#ifndef OMITBAD

static int badStatic = 0;

static int * badSource(int * data) {
    if(badStatic) {
        data = (int *)malloc(10);
        if (data == NULL) {exit(-1);}
    }
    return data;
}

void CWE122_Heap_Based_Buffer_Overflow__CWE131_loop_21_bad() {
    int * data;
    data = NULL;
    badStatic = 1; // true
    data = badSource(data);
    {
        int source[10] = {0};
        size_t i;
        for (i = 0; i < 10; i++) {
            data[i] = source[i]; // POTENTIAL FLAW
        }
        printIntLine(data[0]);
        free(data);
    }
}

#endif /* OMITBAD */

#ifndef OMITGOOD

static int goodG2B1Static = 0;
static int goodG2B2Static = 0;

static int * goodG2B1Source(int * data) {
    if(goodG2B1Static) {
        printLine("Benign, fixed string");
    } else {
        data = (int *)malloc(10 * sizeof(int));
        if (data == NULL) {exit(-1);}
    }
    return data;
}

static void goodG2B1() {
    int * data;
    data = NULL;
    goodG2B1Static = 0; // false
    data = goodG2B1Source(data);
    {
        int source[10] = {0};
        size_t i;
        for (i = 0; i < 10; i++) {
            data[i] = source[i]; // POTENTIAL FLAW
        }
        printIntLine(data[0]);
        free(data);
    }
}

static int * goodG2B2Source(int * data) {
    if(goodG2B2Static) {
        data = (int *)malloc(10 * sizeof(int));
        if (data == NULL) {exit(-1);}
    }
    return data;
}

static void goodG2B2() {
    int * data;
    data = NULL;
    goodG2B2Static = 1; // true
    data = goodG2B2Source(data);
    {
        int source[10] = {0};
        size_t i;
        for (i = 0; i < 10; i++) {
            data[i] = source[i]; // POTENTIAL FLAW
        }
        printIntLine(data[0]);
        free(data);
    }
}

void CWE122_Heap_Based_Buffer_Overflow__CWE131_loop_21_good() {
    goodG2B1();
    goodG2B2();
}

#endif /* OMITGOOD */

int main(int argc, char * argv[]) {
    srand((unsigned)time(NULL));
#ifndef OMITGOOD
    printf("Calling good()...\n");
    CWE122_Heap_Based_Buffer_Overflow__CWE131_loop_21_good();
    printf("Finished good()\n");
#endif /* OMITGOOD */
#ifndef OMITBAD
    printf("Calling bad()...\n");
    CWE122_Heap_Based_Buffer_Overflow__CWE131_loop_21_bad();
    printf("Finished bad()\n");
#endif /* OMITBAD */
    return 0;
}