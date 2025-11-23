```c
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

void CWE190_Integer_Overflow__char_rand_add_54e_badSink(char data);
void CWE190_Integer_Overflow__char_rand_add_54e_goodG2BSink(char data);
void CWE190_Integer_Overflow__char_rand_add_54e_goodB2GSink(char data);

void run_bad(void) {
    char data;
    /* POTENTIAL FLAW: Use the maximum value for char to ensure overflow on add */
    data = CHAR_MAX;
    printf("Running bad case with data: %d\n", data);
    CWE190_Integer_Overflow__char_rand_add_54e_badSink(data);
}

void run_good(void) {
    char data;
    /* FIX: Use a small, safe value to avoid overflow */
    data = 2;
    printf("Running good case with data: %d\n", data);
    CWE190_Integer_Overflow__char_rand_add_54e_goodB2GSink(data);
}

int main(void) {
    /* Seed randomness just in case, though not needed by current paths */
    srand((unsigned)time(NULL));
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```