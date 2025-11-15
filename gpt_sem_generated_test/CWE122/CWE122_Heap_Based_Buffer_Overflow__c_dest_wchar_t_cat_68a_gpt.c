#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>

wchar_t * CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_badData;
wchar_t * CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_goodG2BData;

void CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68b_badSink();
void CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68b_goodG2BSink();

void CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_bad() {
    wchar_t * data;
    data = NULL;
    data = (wchar_t *)malloc(50 * sizeof(wchar_t)); // Small buffer
    if (data == NULL) { exit(-1); }
    data[0] = L'\0'; // null terminate
    CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_badData = data;
    CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68b_badSink();
}

static void goodG2B() {
    wchar_t * data;
    data = NULL;
    data = (wchar_t *)malloc(100 * sizeof(wchar_t)); // Large enough buffer
    if (data == NULL) { exit(-1); }
    data[0] = L'\0'; // null terminate
    CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_goodG2BData = data;
    CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68b_goodG2BSink();
}

int main(int argc, char * argv[]) {
    srand((unsigned)time(NULL));
    
    printf("Testing bad case...\n");
    CWE122_Heap_Based_Buffer_Overflow__c_dest_wchar_t_cat_68_bad();
    printf("Finished bad case.\n");

    printf("Testing good case...\n");
    goodG2B();
    printf("Finished good case.\n");

    return 0;
}