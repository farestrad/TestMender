#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

extern void CWE665_Improper_Initialization__wchar_t_ncat_53d_goodG2BSink(void);
extern void CWE665_Improper_Initialization__wchar_t_ncat_53d_badSink(void);

int main(void) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        alarm(3);
        CWE665_Improper_Initialization__wchar_t_ncat_53d_goodG2BSink();
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
        CWE665_Improper_Initialization__wchar_t_ncat_53d_badSink();
        exit(0);
    } else {
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            printf("BAD: PASS\n");
        } else {
            printf("BAD: FAIL\n");
        }
    }

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0 && WIFEXITED(status) && WEXITSTATUS(status) != 0) {
        exit(0);
    } else {
        exit(1);
    }
}