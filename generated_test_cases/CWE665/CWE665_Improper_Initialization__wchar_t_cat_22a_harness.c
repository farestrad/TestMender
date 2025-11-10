#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

extern void goodG2B1(void);
extern void bad(void);

int main() {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        alarm(3);
        goodG2B1();
        exit(0);
    } else if (pid < 0) {
        perror("fork");
        exit(1);
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
        bad();
        exit(0);
    } else if (pid < 0) {
        perror("fork");
        exit(1);
    } else {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("BAD: PASS\n");
        } else {
            printf("BAD: FAIL\n");
        }
    }

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0 && WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        exit(0);
    } else {
        exit(1);
    }
}