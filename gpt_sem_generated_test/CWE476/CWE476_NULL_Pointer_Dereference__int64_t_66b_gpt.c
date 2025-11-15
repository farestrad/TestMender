#include <stdio.h>
#include <stdint.h>

void printLongLongLine(int64_t data) {
    printf("%lld\n", data);
}

void printLine(const char *str) {
    printf("%s\n", str);
}

#ifndef OMITBAD
void CWE476_NULL_Pointer_Dereference__int64_t_66b_badSink(int64_t * dataArray[]) {
    int64_t * data = dataArray[2];
    printLongLongLine(*data);
}
#endif /* OMITBAD */

#ifndef OMITGOOD
void CWE476_NULL_Pointer_Dereference__int64_t_66b_goodB2GSink(int64_t * dataArray[]) {
    int64_t * data = dataArray[2];
    if (data != NULL) {
        printLongLongLine(*data);
    } else {
        printLine("data is NULL");
    }
}
#endif /* OMITGOOD */

void testBad() {
    int64_t *dataArray[5];
    dataArray[2] = NULL; // Bad case, data is NULL
    printf("Running test for bad sink:\n");
    CWE476_NULL_Pointer_Dereference__int64_t_66b_badSink(dataArray);
}

void testGoodB2G() {
    int64_t value = 42;
    int64_t *dataArray[5];
    dataArray[2] = &value; // Good case, data is initialized
    printf("Running test for goodB2G sink:\n");
    CWE476_NULL_Pointer_Dereference__int64_t_66b_goodB2GSink(dataArray);
}

int main() {
    testBad();
    testGoodB2G();
    return 0;
}