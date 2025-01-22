#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int server, newSock;
    char buffer[1024];
    struct sockaddr_in servAddr;
    struct sockaddr_storage store;
    socklen_t addrSize;

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

    while (1) {
        // Accepting new client connection
        newSock = accept(server, (struct sockaddr *)&store, &addrSize);
        if (newSock < 0) {
            perror("Accept failed");
            continue; // Skip to the next iteration if accepting fails
        }

        printf("Client connected.\n");

        while (1) {
            // Clear buffer before receiving data
            memset(buffer, 0, sizeof(buffer));

            // Receive data from the client
            int valread = recv(newSock, buffer, sizeof(buffer) - 1, 0);
            if (valread > 0) {
                buffer[valread] = '\0'; // Null-terminate the string
                printf("Client: %s\n", buffer);

                // If client sends 'exit', close the connection
                if (strcmp(buffer, "exit") == 0) {
                    printf("Client exited. Closing connection...\n");
                    break; // Exit the inner loop to close the connection
                }

                // Prompt server to send a message
                memset(buffer, 0, sizeof(buffer));
                printf("Enter message to client: ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

                send(newSock, buffer, strlen(buffer), 0);

                if (strcmp(buffer, "exit") == 0) {
                    printf("Exiting server...\n");
                    close(newSock);
                    close(server); // Close the server socket
                    return 0; // Exit the program
                }
            } else if (valread == 0) {
                printf("Client disconnected.\n");
                break;
            } else {
                perror("recv failed");
                break;
            }
        }

        printf("Waiting for new client...\n");
    }
    close(newSock);
    close(server); // Close the server socket when exiting
    return 0;
}
