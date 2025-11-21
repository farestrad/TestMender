#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

extern void good(void);
extern void bad(void);

int main() {
    pid_t pid;
    int status;

    // Fork and run GOOD
    pid = fork();
    if (pid == 0) {
        alarm(3);
        CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_declare_ncpy_07_good();
        exit(0); // Should not reach here
    } else {
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("GOOD: PASS\n");
        } else {
            printf("GOOD: FAIL\n");
        }
    }

    // Fork and run BAD
    pid = fork();
    if (pid == 0) {
        alarm(3);
        CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_declare_ncpy_07_bad();
        exit(0); // Should not reach here
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