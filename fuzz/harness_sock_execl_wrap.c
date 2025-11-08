#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

/* ===== Fuzzer-provided bytes ===== */
static const unsigned char* FBUF = NULL;
static size_t FLEN = 0, FPOS = 0;

/* ===== Stub libc calls ===== */
#define socket  my_socket
#define connect my_connect
#define recv    my_recv
#define close   my_close
#define execl   my_execl

static int my_socket(int d, int t, int p) { (void)d; (void)t; (void)p; return 3; }
static int my_connect(int fd, const struct sockaddr* a, socklen_t l) { (void)fd; (void)a; (void)l; return 0; }
static ssize_t my_recv(int fd, void* buf, size_t len, int fl) {
    (void)fd; (void)fl;
    if (!FBUF || FPOS >= FLEN) return 0;
    size_t rem = FLEN - FPOS;
    size_t take = rem < len ? rem : len;
    memcpy(buf, FBUF + FPOS, take);
    FPOS += take;
    return (ssize_t)take;
}
static int my_close(int fd) { (void)fd; return 0; }

/* stub execl so it never runs real commands */
static int my_execl(const char* path, const char* arg0, ...) {
    (void)path; (void)arg0;
    errno = ENOSYS;
    return -1;
}

/* Avoid Juliet's main() */
#undef INCLUDEMAIN

/* include the Juliet testcase */
#include TESTFILE

int CWE78_OS_Command_Injection__char_connect_socket_execl_01_bad(void);

/* libFuzzer entrypoint */
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    FBUF = data; FLEN = size; FPOS = 0;
    CWE78_OS_Command_Injection__char_connect_socket_execl_01_bad();
    return 0;
}

/* AFL++ entrypoint */
#ifdef AFL_MAIN
int main(void) {
    static unsigned char buf[1<<16];
    size_t n = fread(buf, 1, sizeof(buf), stdin);
    FBUF = buf; FLEN = n; FPOS = 0;
    CWE78_OS_Command_Injection__char_connect_socket_execl_01_bad();
    return 0;
}
#endif
