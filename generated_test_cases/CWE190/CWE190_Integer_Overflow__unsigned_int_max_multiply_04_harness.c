#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

extern void goodB2G1(void);
extern void bad(void);

int main() {
    pid_t pid;
    int status;

    // Test GOOD
    pid = fork();
    if (pid == 0) {
        alarm(3);
        goodB2G1();
        exit(0);
    } else {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("GOOD: PASS\n");
        } else {
            printf("GOOD: FAIL\n");
        }
    }

    // Test BAD
    pid = fork();
    if (pid == 0) {
        alarm(3);
        bad();
        exit(0);
    } else {
        waitpid(pid, &status, 0);
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