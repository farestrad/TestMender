```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <unistd.h>

#define TCP_PORT 27015
#define IP_ADDRESS "127.0.0.1"
#define CHAR_ARRAY_SIZE (3 * sizeof(data) + 2)
#define CLOSE_SOCKET closesocket

// Function declarations
void testCWE121();

int main() {
    testCWE121();
    return 0;
}

void testCWE121() {
    int data;
    data = -1;

    // Simulate a connection and data receipt
    {
        int recvResult;
        struct sockaddr_in service;
        SOCKET connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (connectSocket != INVALID_SOCKET) {
            memset(&service, 0, sizeof(service));
            service.sin_family = AF_INET;
            service.sin_addr.s_addr = inet_addr(IP_ADDRESS);
            service.sin_port = htons(TCP_PORT);
            if (connect(connectSocket, (struct sockaddr*)&service, sizeof(service)) != SOCKET_ERROR) {
                char inputBuffer[CHAR_ARRAY_SIZE];
                recvResult = recv(connectSocket, inputBuffer, CHAR_ARRAY_SIZE - 1, 0);
                if (recvResult > 0) {
                    inputBuffer[recvResult] = '\0';
                    data = atoi(inputBuffer);
                }
            }
            CLOSE_SOCKET(connectSocket);
        }
    }

    // Buffer to test the overflow
    int buffer[10] = { 0 };

    // Vulnerable code path
    if (data >= 0) {
        // BAD: Attempting to write to an index that may be out-of-bounds
        buffer[data] = 1; // CWE-121 vulnerability
        printf("Buffer index set at %d\n", data);
    } else {
        printf("ERROR: Array index is negative.\n");
    }

    // Print buffer to verify contents
    for (int i = 0; i < 10; i++) {
        printf("buffer[%d] = %d\n", i, buffer[i]);
    }

    // Securely erase sensitive data from inputBuffer
    memset_s(inputBuffer, sizeof(inputBuffer), 0, sizeof(inputBuffer));
}
```