#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

extern void goodG2B(void);
extern void CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_alloca_memcpy_61b_badSource(void);

int main() {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        alarm(3);
        goodG2B();
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
        CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_alloca_memcpy_61b_badSource();
        exit(0);
    } else {
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            printf("BAD: PASS\n");
        } else {
            printf("BAD: FAIL\n");
        }
    }

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        exit(1);
    } else {
        exit(0);
    }
}