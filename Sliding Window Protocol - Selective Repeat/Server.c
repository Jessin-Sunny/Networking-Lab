#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define TOTAL_PACKETS 5
#define TIME 2

int main() {
    int server, newSock;
    char buffer[1024];
    struct sockaddr_in servAddr;
    struct sockaddr_storage store;
    socklen_t addrSize;
    struct timeval timeout = {TIME,0};
    int received_count = 0, seq;
    int received[TOTAL_PACKETS] = {0};

    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) {
        perror("Socket failed");
        return -1;
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(6265);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(server, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("Bind failed");
        return -1;
    }

    if (listen(server, 5) == 0) {
        printf("Server is listening...\n");
    } else {
        perror("Listen failed");
        return -1;
    }

    addrSize = sizeof(servAddr);
    newSock = accept(server, (struct sockaddr*)&servAddr, &addrSize);
    setsockopt(newSock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    while(received_count < TOTAL_PACKETS)
    {
        if (recv(newSock, &seq, sizeof(seq), 0) > 0)
        {
            printf("Received: %d\n", seq);
            if (seq >= 0 && seq < TOTAL_PACKETS && !received[seq])
            {
                received[seq] = 1;
                received_count++;
            }
            send(newSock, &seq, sizeof(seq), 0); // ACK that specific seq
            printf("Sent ACK: %d\n", seq);
        }
        else
        {
            printf("Timeout...\n");
        }
    }

    close(newSock);
    close(server); // Close the server socket when exiting
    return 0;
}
