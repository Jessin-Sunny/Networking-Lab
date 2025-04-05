#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#define WINDOW_SIZE 4
#define TOTAL_PACKETS 5

int main()
{
    int client, ack, seq, base, i;
    struct sockaddr_in servAddr;

    client = socket(AF_INET, SOCK_STREAM, 0);
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(6265);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        printf("Connection failed...\n");
        return -1;
    }

    printf("Connected to Server.....\n");

    base = 0;           //used as starting packet seqence number to be sent up to WINDOW_SIZE packets (base to base + WINDOW_SIZE  packets)
    while(base < TOTAL_PACKETS)
    {
        for(i=0; i < WINDOW_SIZE && (base + i) < TOTAL_PACKETS; i++)
        {
            seq = base + i;
            send(client, &seq, sizeof(seq), 0);
            printf("Sent Packet with Sequence Number %d\n", seq);
        }
        if(recv(client, &ack, sizeof(ack), 0) > 0)
        {
            printf("Received Acknowledgement: %d\n", ack);
            base = ack;
        }
        else
        {
            printf("Timeout!!!\n");
        }
    }

    close(client);
    return 0;
}
