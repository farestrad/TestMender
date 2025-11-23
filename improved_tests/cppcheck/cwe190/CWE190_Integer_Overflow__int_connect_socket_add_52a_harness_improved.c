```c
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Prototypes for the Juliet entry functions
void CWE190_Integer_Overflow__int_connect_socket_add_52_bad(void);
void CWE190_Integer_Overflow__int_connect_socket_add_52_good(void);

/*
 * The Juliet "connect_socket" variants read their initial integer from a TCP socket.
 * For the BAD run, we need to ensure the integer sent is a large value (e.g. INT_MAX) so overflow occurs on add.
 * For the GOOD run, we supply a small positive safe integer so no overflow can occur.
 * 
 * We create a child process to act as a local TCP server to supply the integer.
 * The port used is the default Juliet port: 39543.
 */

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>
#pragma comment(lib, "ws2_32.lib")
#define CLOSESOCKET closesocket
typedef SOCKET socket_t;
#else
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#define CLOSESOCKET close
typedef int socket_t;
#endif

#define TEST_PORT 39543
#define SERVER_BACKLOG 5

static int start_test_server_and_send(int val_to_send)
{
#ifdef _WIN32
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2,2), &wsa_data) != 0) {
        fprintf(stderr, "WSAStartup failed\n");
        return 0;
    }
#endif

    socket_t listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenfd < 0) {
        perror("socket");
        return 0;
    }

    int optval = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(TEST_PORT);

    if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind");
        CLOSESOCKET(listenfd);
        return 0;
    }

    if (listen(listenfd, SERVER_BACKLOG) < 0) {
        perror("listen");
        CLOSESOCKET(listenfd);
        return 0;
    }

    socket_t clientfd = -1;
    struct sockaddr_in cli_addr;
#if defined(_WIN32)
    int cli_len = sizeof(cli_addr);
#else
    socklen_t cli_len = sizeof(cli_addr);
#endif

    clientfd = accept(listenfd, (struct sockaddr*)&cli_addr, &cli_len);
    if (clientfd < 0) {
        perror("accept");
        CLOSESOCKET(listenfd);
        return 0;
    }

    char buffer[32];
    int len = snprintf(buffer, sizeof(buffer), "%d\n", val_to_send);
    if (send(clientfd, buffer, len, 0) < 0) {
        perror("send");
        CLOSESOCKET(clientfd);
        CLOSESOCKET(listenfd);
        return 0;
    }

    CLOSESOCKET(clientfd);
    CLOSESOCKET(listenfd);
#ifdef _WIN32
    WSACleanup();
#endif
    return 1;
}

#ifdef _WIN32
#define FORK_TYPE intptr_t
#else
#define FORK_TYPE pid_t
#endif

static void run_with_server(int send_value, void (*testfunc)(void))
{
    /*
     * Fork/spawn a process for the server, or create a thread.
     * Since Juliet runs as a single process and expects to connect to localhost:39543,
     * we must run our local server in parallel and ensure the port is ready first.
     */

#if defined(_WIN32)
    // Use CreateThread for concurrency on Windows
    #include <windows.h>
    typedef struct { int value; } server_args_t;
    static DWORD WINAPI server_thr(LPVOID arg) {
        server_args_t *sargs = (server_args_t*)arg;
        start_test_server_and_send(sargs->value);
        return 0;
    }
    server_args_t args;
    args.value = send_value;
    HANDLE hThread = CreateThread(NULL, 0, server_thr, &args, 0, NULL);
    if (!hThread) {
        fprintf(stderr, "CreateThread failed\n");
        exit(1);
    }
    // Give server a moment to start listening
    Sleep(120);

    // Now call the Juliet test, which will connect to the test server
    testfunc();

    // Wait for server thread to finish
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
#else
    // Use fork on POSIX
    FORK_TYPE pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // Child process runs the server
        start_test_server_and_send(send_value);
        _exit(0);
    } else {
        // Give child/server a moment to begin listening
        usleep(120*1000);

        // Parent runs the test function, which will connect to the socket
        testfunc();

        // Wait for server process to finish
        int wstatus = 0;
        waitpid(pid, &wstatus, 0);
    }
#endif
}

void run_bad(void) {
    printf("Running BAD scenario...\n");
    // This should trigger overflow in the sink: set data = INT_MAX or similar
    run_with_server(INT_MAX, CWE190_Integer_Overflow__int_connect_socket_add_52_bad);
}

void run_good(void) {
    printf("Running GOOD scenario...\n");
    // This should NOT trigger overflow: set data to a small, safe non-zero integer
    run_with_server(2, CWE190_Integer_Overflow__int_connect_socket_add_52_good);
}

int main(void) {
#ifdef TEST_MODE_BAD
    run_bad();
#else
    run_good();
#endif
    return 0;
}
```