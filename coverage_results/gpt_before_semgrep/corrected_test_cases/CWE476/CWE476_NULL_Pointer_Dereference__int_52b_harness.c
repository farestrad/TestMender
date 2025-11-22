#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

extern void CWE476_NULL_Pointer_Dereference__int_52c_goodG2BSink(void);
extern void CWE476_NULL_Pointer_Dereference__int_52c_badSink(void);

int main() {
    pid_t pid;
    int status;

    // Fork for GOOD
    pid = fork();
    if (pid == 0) {
        alarm(3);
        CWE476_NULL_Pointer_Dereference__int_52c_goodG2BSink();
        exit(0);
    } else {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("GOOD: PASS\n");
        } else {
            printf("GOOD: FAIL\n");
        }
    }

    // Fork for BAD
    pid = fork();
    if (pid == 0) {
        alarm(3);
        CWE476_NULL_Pointer_Dereference__int_52c_badSink();
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