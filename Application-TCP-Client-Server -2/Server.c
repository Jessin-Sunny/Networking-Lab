#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define size 25

int a[size][size], b[size][size], c[size][size], n, m, p, q, r, s;

void multiply(int a[size][size], int b[size][size], int c[size][size], int n, int m, int p, int q)
{
    int i,j,k;
    if( m != p)
    {
        printf("Matrix Multiplication is not possible\n");
        r = 0;
        s = 0;
    }
    else
    {
        for(i=0; i<n; i++)
        {
            for(j=0; j<q; j++)
            {
                c[i][j] = 0;
                for(k=0; k<m; k++)
                {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
        r = n;
        s = q;
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
    int server, valread, newSock;
    struct sockaddr_in servAddr;
    struct sockaddr_storage store;
    socklen_t addrSize;

    server = socket(AF_INET, SOCK_STREAM, 0);
    if(server < 0)
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

    if(bind(server, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0)
    {
        printf("Binding Failed\n");
        close(server);
        return -1;
    }
    else
    {
        printf("Binding Successfull\n");
    }

    if(listen(server, 5) == 0)
    {
        printf("Server is listening...\n");
    }

    addrSize = sizeof(servAddr);
    while(1)
    {
        newSock = accept(server, (struct sockaddr*)&store, &addrSize);
        if(newSock < 0)
        {
            printf("Accepting Failed\n");
            continue;
        }

        printf("Client Connected\n");

        //first matrix row size
        recv(newSock, &n, sizeof(int), 0);

        //first matrix column size
        recv(newSock, &m, sizeof(int), 0);

        //first matrix
        recv(newSock, a, sizeof(int) * size * size, 0);

        //second matrix row size
        recv(newSock, &p, sizeof(int), 0);

        //second matrix column size
        recv(newSock, &q, sizeof(int), 0);

        //second matrix
        recv(newSock, b, sizeof(int) * size * size, 0);

        //multiplication
        multiply(a, b, c, n, m, p, q);

        //printing resultant matrix
        display(c, r, s);
        //sending to client
        send(newSock, c, sizeof(int) * size * size, 0);
    }
    close(newSock);
    close(server);
    return 0;
}
