#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <assert.h>

#define BUFFER_SIZE 50
#define OVERSIZE_BUFFER 100

// Simulating the bad sink function
void CWE121_Stack_Based_Buffer_Overflow__src_wchar_t_declare_cat_54d_badSink(wchar_t * data) {
    wchar_t dest[BUFFER_SIZE];
    wcscat(dest, data); // Potential buffer overflow if data is too large
}

// Simulating the good sink function
void CWE121_Stack_Based_Buffer_Overflow__src_wchar_t_declare_cat_54d_goodG2BSink(wchar_t * data) {
    wchar_t dest[BUFFER_SIZE];
    wcscat(dest, data); // It should be safe as long as data is small
}

// Bad test case
void test_bad_case() {
    wchar_t data[OVERSIZE_BUFFER];
    wchar_t *oversized_data = L"This is a long string that exceeds the safe buffer size of the destination buffer.";
    wcscpy(data, oversized_data);

    // Call the bad sink
    CWE121_Stack_Based_Buffer_Overflow__src_wchar_t_declare_cat_54d_badSink(data);
    printf("Bad test executed. Check for buffer overflow.\n");
}

// Good test case
void test_good_case() {
    wchar_t data[BUFFER_SIZE];
    wchar_t *safe_data = L"Short string.";
    wcscpy(data, safe_data);

    // Call the good sink
    CWE121_Stack_Based_Buffer_Overflow__src_wchar_t_declare_cat_54d_goodG2BSink(data);
    printf("Good test executed. Safe buffer usage confirmed.\n");
}

int main() {
    printf("Running tests...\n");
    test_bad_case(); // Should show overflow behavior
    test_good_case(); // Should complete without issues
    return 0;
}