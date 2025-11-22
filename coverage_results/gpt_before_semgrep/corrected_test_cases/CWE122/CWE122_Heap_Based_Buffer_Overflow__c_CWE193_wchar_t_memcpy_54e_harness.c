#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

extern void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_wchar_t_memcpy_54e_goodG2BSink(void);
extern void CWE122_Heap_Based_Buffer_Overflow__c_CWE193_wchar_t_memcpy_54e_badSink(void);

int main() {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        alarm(3);
        CWE122_Heap_Based_Buffer_Overflow__c_CWE193_wchar_t_memcpy_54e_goodG2BSink();
        exit(0);
    } else {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("GOOD: PASS\n");
        } else {
            printf("GOOD: FAIL\n");
        }
    }

    pid = fork();
    if (pid == 0) {
        alarm(3);
        CWE122_Heap_Based_Buffer_Overflow__c_CWE193_wchar_t_memcpy_54e_badSink();
        exit(0);
    } else {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            printf("BAD: PASS\n");
        } else {
            printf("BAD: FAIL\n");
        }
    }

    return (WIFEXITED(status) && WEXITSTATUS(status) == 0) ? 0 : 1;
}