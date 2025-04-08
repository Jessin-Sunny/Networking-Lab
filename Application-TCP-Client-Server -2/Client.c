#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define size 25

int a[size][size], b[size][size], c[size][size], n, m, p, q, r, s;
int matrix[size][size];


void read_matrix(int a[size][size], int* x, int* y)
{
    int i,j;

    printf("How many rows ? : ");
    scanf("%d",x);
    printf("How many columns ? : ");
    scanf("%d",y);

    printf("Enter the matrix\n");
    for(i=0; i<*x; i++)
    {
        for(j=0; j<*y; j++)
        {
            scanf("%d",&a[i][j]);
        }
    }
}

void display(int a[size][size], int x, int y)
{
    int i,j;
    for(i=0; i<x; i++)
    {
        for(j=0; j<y; j++)
        {
            printf("%d ",a[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    int client, valread, newSock;
    struct sockaddr_in servAddr;
    socklen_t addrSize;

    client = socket(AF_INET, SOCK_STREAM, 0);
    if(client < 0)
    {
        printf("Socket Creation Failed\n");
        return -1;
    }
    else
    {
        printf("Socket Creation Successfull\n");
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(6125);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.36");

    if(connect(client, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0)
    {
        printf("Connection Failed\n");
        close(client);
        return -1;
    }
    else
    {
        printf("Connection Successfull\n");
    }


    addrSize = sizeof(servAddr);
    while(1)
    {
        //int a[size][size]  = {{1, 2, 3},{4, 5, 6}};
        //input the first matrix
        printf("Enter the First Matrix\n");
        read_matrix(a, &n, &m);
        //input second matrix
        //int b[size][size] = {{7, 8 }, {9, 10}, {11, 12}};
        printf("Enter the Second Matrix\n");
         read_matrix(b, &p, &q);

        if( m != p)
        {
            printf("Matrix Multiplication is not possible\n");
            continue;
        }
        //send first matrix row size to server
        send(client,  &n, sizeof(int), 0);
        //send first matrix column size to server
        send(client,  &m, sizeof(int), 0);
        //send first matrix to server
        send(client,  a, sizeof(int) * size * size, 0);

        //send second matrix row size to server
        send(client,  &p, sizeof(int), 0);
        //send second matrix column size to server
        send(client,  &q, sizeof(int), 0);
        //send second matrix to server
        send(client,  b, sizeof(int)* size * size, 0);

        //recieving the resultant matrix from server
        recv(client, c, sizeof(int) * size * size, 0);
        printf("Resultant Matrix\n");
        display(c, n, q);

        char ch;
        printf("\nDo you want to contine ? (y/n) : ");
        scanf(" %c",&ch);
        if(ch == 'y' || ch =='Y')
        {
            printf("Exiting Client...\n");
            break;
        }
    }
    close(client);
    return 0;
}
