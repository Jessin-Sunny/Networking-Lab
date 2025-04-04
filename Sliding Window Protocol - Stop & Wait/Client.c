#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MSG_LEN 5

int main()
{
    int client, ack, seq;
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

    //Packet whose seqence number  is determined from acknowledgement received from server will be sent
    seq = 0;
    while(seq < MSG_LEN)
    {
        send(client, &seq, sizeof(seq), 0);
        printf("Sending Packet with Sequence Number %d\n", seq);
        if (recv(client, &ack, sizeof(ack), 0) > 0)
        {
            printf("Received Acknowledgement: %d\n", ack);
            seq = ack;
        }
        else
        {
            printf("Timeot!!! Error in Receiving Packet\n");
        }
    }

    close(client);
    return 0;
}
