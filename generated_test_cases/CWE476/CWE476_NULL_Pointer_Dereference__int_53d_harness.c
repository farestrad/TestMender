#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

extern void CWE476_NULL_Pointer_Dereference__int_53d_goodG2BSink(void);
extern void CWE476_NULL_Pointer_Dereference__int_53d_badSink(void);

int main(void) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        alarm(3);
        CWE476_NULL_Pointer_Dereference__int_53d_goodG2BSink();
        exit(0);
    } else {
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("GOOD: PASS\n");
        } else {
            printf("GOOD: FAIL\n");
        }
    }

    pid = fork();
    if (pid == 0) {
        alarm(3);
        CWE476_NULL_Pointer_Dereference__int_53d_badSink();
        exit(0);
    } else {
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            printf("BAD: PASS\n");
        } else {
            printf("BAD: FAIL\n");
        }
    }

    return (WIFEXITED(status) && WEXITSTATUS(status) == 0) ? 0 : 1;
}