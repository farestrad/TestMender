#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

extern void goodG2BSink(void);
extern void bad(void);

int main() {
    pid_t pid;
    int status;

    // Fork for GOOD
    pid = fork();
    if (pid == 0) {
        alarm(3);
        goodG2BSink();
        exit(0);
    } else {
        wait(&status);
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
        bad();
        exit(0);
    } else {
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            printf("BAD: PASS\n");
        } else if (WIFSIGNALED(status)) {
            printf("BAD: PASS\n");
        } else {
            printf("BAD: FAIL\n");
        }
    }

    return (WIFEXITED(status) && WEXITSTATUS(status) == 0) ? 0 : 1;
}