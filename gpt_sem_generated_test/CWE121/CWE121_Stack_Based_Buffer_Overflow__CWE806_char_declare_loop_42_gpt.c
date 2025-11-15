#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

void printLine(char * line) {
    printf("%s\n", line);
}

static char * badSource(char * data) {
    memset(data, 'A', 100-1); /* fill with 'A's */
    data[100-1] = '\0'; /* null terminate */
    return data;
}

void CWE121_Stack_Based_Buffer_Overflow__CWE806_char_declare_loop_42_bad() {
    char * data;
    char dataBuffer[100];
    data = dataBuffer;
    data = badSource(data);
    {
        char dest[50] = "";
        size_t i, dataLen;
        dataLen = strlen(data);
        for (i = 0; i < dataLen; i++) {
            if (i < sizeof(dest)-1) {
                dest[i] = data[i];
            } else {
                printf("Buffer overflow detected!\n");
                return;
            }
        }
        dest[50-1] = '\0';
        printLine(data);
    }
}

static char * goodG2BSource(char * data) {
    memset(data, 'A', 50-1); /* fill with 'A's */
    data[50-1] = '\0'; /* null terminate */
    return data;
}

static void goodG2B() {
    char * data;
    char dataBuffer[100];
    data = dataBuffer;
    data = goodG2BSource(data);
    {
        char dest[50] = "";
        size_t i, dataLen;
        dataLen = strlen(data);
        for (i = 0; i < dataLen; i++) {
            if (i < sizeof(dest)-1) {
                dest[i] = data[i];
            } else {
                printf("Buffer overflow detected!\n");
                return;
            }
        }
        dest[50-1] = '\0';
        printLine(data);
    }
}

int main(int argc, char * argv[]) {
    srand((unsigned)time(NULL));
    printLine("Calling good()...");
    goodG2B();
    printLine("Finished good()");
    printLine("Calling bad()...");
    CWE121_Stack_Based_Buffer_Overflow__CWE806_char_declare_loop_42_bad();
    printLine("Finished bad()");
    return 0;
}