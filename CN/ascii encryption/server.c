#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  // for inet_ntoa
#include <unistd.h>     // for read and close

#define PORT 8080
#define MAX_MSG 256

void decrypt_message(char *msg) {
    int i;
    for (i = 0; i < strlen(msg); i++) {
        msg[i] -= 4; // subtract 4 to decrypt
    }
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[MAX_MSG];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket creation failed");
        exit(1);
    }

    // Set address and port number for the server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the address and port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("binding failed");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_fd, 1) < 0) {
        perror("listening failed");
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept incoming connection
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("accepting failed");
        exit(1);
    }

    printf("Connection accepted from client IP address %s and port %d...\n",
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    // Receive encrypted message from client
    read(client_fd, buffer, MAX_MSG);
    printf("Encrypted message received: %s\n", buffer);

    // Decrypt message
    decrypt_message(buffer);
    printf("Decrypted message: %s\n", buffer);

    // Close sockets
    close(client_fd);
    close(server_fd);

    return 0;
}