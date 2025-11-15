#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALLOCA(size) alloca(size)
#define printIntLine(x) printf("%d\n", (x))
#define printLine(x) printf("%s\n", (x))

int globalReturnsTrue() {
    return 1;
}

int globalReturnsFalse() {
    return 0;
}

void CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_11_bad() {
    int *data;
    int *dataBadBuffer = (int *)ALLOCA(50 * sizeof(int));
    int *dataGoodBuffer = (int *)ALLOCA(100 * sizeof(int));
    if (globalReturnsTrue()) {
        data = dataBadBuffer;
    }
    {
        int source[100] = {0}; // fill with 0's
        // POTENTIAL FLAW: Possible buffer overflow if data < 100
        memmove(data, source, 100 * sizeof(int));
        printIntLine(data[0]); // Check if we have a potential overflow
    }
}

void CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_11_good() {
    int *data;
    int *dataGoodBuffer = (int *)ALLOCA(100 * sizeof(int));
    
    // Properly allocate a large buffer
    data = dataGoodBuffer;
    {
        int source[100] = {0}; // fill with 0's
        // Safe as data is sufficiently large
        memmove(data, source, 100 * sizeof(int));
        printIntLine(data[0]); // Should print 0 without issue
    }
}

int main(int argc, char *argv[]) {
    printf("Calling bad()...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_11_bad();
    printf("Finished bad()\n");

    printf("Calling good()...\n");
    CWE121_Stack_Based_Buffer_Overflow__CWE805_int_alloca_memmove_11_good();
    printf("Finished good()\n");

    return 0;
}