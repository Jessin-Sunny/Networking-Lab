#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define TOTAL_PACKETS 5
#define TIME 2

int main()
{
    int client;
    char buffer[1024];
    struct sockaddr_in servAddr;
    struct timeval timeout = {TIME, 0};
    int ackd[TOTAL_PACKETS] = {0};
    int i, all_done, ack;

    client = socket(AF_INET, SOCK_STREAM, 0);
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(6265);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    printf("Connected to server.\n");
    while(1)
    {
        all_done = 1;
        for(i = 0; i < TOTAL_PACKETS; i++)
        {
            if(!ackd[i])
            {
                all_done = 0;
                send(client, &i, sizeof(int), 0);
                printf("Sent Packet with Sequence Number %d\n", i);
            }
        }
        if(all_done)
        {
            break;
        }
        while(recv(client, &ack, sizeof(int), 0) > 0)
        {
            printf("Received Acknowledgement: %d\n", ack);
            if (ack >= 0 && ack < TOTAL_PACKETS)
            {
                ackd[ack] = 1;
            }
        }
    }
    close(client);
    return 0;
}
