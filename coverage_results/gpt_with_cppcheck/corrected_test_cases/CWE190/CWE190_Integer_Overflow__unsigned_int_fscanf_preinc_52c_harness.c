// ```c
#include <stdio.h>
#include <limits.h>

/* Prototypes for the Juliet entry functions */
void CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_52c_badSink(unsigned int data);
void CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_52c_goodG2BSink(unsigned int data);
void CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_52c_goodB2GSink(unsigned int data);

void run_bad(void) {
    unsigned int data;
    
    /* BAD CASE: Simulating an input that could cause overflow */
    printf("Enter a number: ");
    scanf("%u", &data); // Simulate bad input from stdin
    
    CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_52c_badSink(data);
}

void run_good(void) {
    unsigned int data;

    /* GOOD CASE: Using a safe value */
    data = 2; // Set data to a small, non-zero number
    CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_52c_goodG2BSink(data);

    /* Testing the goodB2G path */
    data = UINT_MAX; // Simulate bad input that would cause overflow but should be handled safely
    CWE190_Integer_Overflow__unsigned_int_fscanf_preinc_52c_goodB2GSink(data);
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
// ```