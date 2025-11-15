#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define SNPRINTF _snprintf
#else
#define SNPRINTF snprintf
#endif

void test_heap_buffer_overflow() {
    char *data;
    data = (char *)malloc(100 * sizeof(char));
    if (data == NULL) { exit(-1); }

    // Initialize data as a large buffer that is larger than the small buffer used in the sink
    memset(data, 'A', 100 - 1);  // fill with 'A's
    data[100 - 1] = '\0';          // null terminate

    char dest[50] = "";
    // POTENTIAL FLAW: Possible buffer overflow if data is larger than dest
    int result = SNPRINTF(dest, sizeof(dest), "%s", data);
    
    // Check if buffer overflow occurred
    if (result < 0 || result >= sizeof(dest)) {
        printf("Buffer overflow detected! Length of data: %d\n", result);
    } else {
        printf("Data written safely: %s\n", dest);
    }
    
    free(data);
}

void test_good_case() {
    char *data;
    data = (char *)malloc(100 * sizeof(char));
    if (data == NULL) { exit(-1); }

    // Initialize data as a small buffer
    memset(data, 'A', 50 - 1);  // fill with 'A's
    data[50 - 1] = '\0';          // null terminate

    char dest[50] = "";
    // Safe snprintf usage
    int result = SNPRINTF(dest, sizeof(dest), "%s", data);

    // Check if buffer overflow occurred
    if (result < 0 || result >= sizeof(dest)) {
        printf("Buffer overflow detected! Length of data: %d\n", result);
    } else {
        printf("Data written safely: %s\n", dest);
    }
    
    free(data);
}

int main() {
    printf("Running test for heap buffer overflow...\n");
    test_heap_buffer_overflow();
    printf("Running test for good case...\n");
    test_good_case();
    return 0;
}