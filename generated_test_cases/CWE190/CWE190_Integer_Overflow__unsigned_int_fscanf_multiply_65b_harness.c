#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

extern void CWE190_Integer_Overflow__unsigned_int_fscanf_multiply_65b_goodG2BSink(void);
extern void CWE190_Integer_Overflow__unsigned_int_fscanf_multiply_65b_badSink(void);

int main() {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        alarm(3);
        CWE190_Integer_Overflow__unsigned_int_fscanf_multiply_65b_goodG2BSink();
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
        CWE190_Integer_Overflow__unsigned_int_fscanf_multiply_65b_badSink();
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