```c
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

void CWE190_Integer_Overflow__int64_t_rand_add_14_bad(void);
void CWE190_Integer_Overflow__int64_t_rand_add_14_good(void);

void run_bad(void) {
    printf("Running bad case...\n");
    CWE190_Integer_Overflow__int64_t_rand_add_14_bad();
}

void run_good(void) {
    printf("Running good case...\n");
    CWE190_Integer_Overflow__int64_t_rand_add_14_good();
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```