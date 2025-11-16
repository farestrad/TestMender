```c
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

void CWE190_Integer_Overflow__char_rand_add_54e_badSink(char data);
void CWE190_Integer_Overflow__char_rand_add_54e_goodG2BSink(char data);
void CWE190_Integer_Overflow__char_rand_add_54e_goodB2GSink(char data);

void run_bad(void) {
    char data;
    // Set data to a random value, simulating a bad source
    data = (char)(rand() % CHAR_MAX);
    printf("Running bad scenario with data: %d\n", data);
    CWE190_Integer_Overflow__char_rand_add_54e_badSink(data);
}

void run_good(void) {
    char data;
    // Set data to a small, non-zero number simulating a good source
    data = 2;
    printf("Running good scenario with data: %d\n", data);
    CWE190_Integer_Overflow__char_rand_add_54e_goodB2GSink(data);
}

int main(void) {
    srand((unsigned)time(NULL));

#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```