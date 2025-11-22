#include <stdio.h>

typedef struct {
    int intOne;
    int intTwo;
} twoIntsStruct;

void CWE476_NULL_Pointer_Dereference__struct_07_bad(void);
void CWE476_NULL_Pointer_Dereference__struct_07_good(void);

void run_bad(void) {
    printf("Running bad case...\n");
    CWE476_NULL_Pointer_Dereference__struct_07_bad();
}

void run_good(void) {
    printf("Running good case...\n");
    CWE476_NULL_Pointer_Dereference__struct_07_good();
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}