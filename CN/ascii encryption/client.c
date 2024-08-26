#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  // for inet_ntoa
#include <unistd.h>     // for close

#define PORT 8080
#define MAX_MSG 256

void encrypt_message(char *msg) {
    int i;
    for (i = 0; i < strlen(msg); i++) {
        msg[i] += 4; // add 4 to encrypt
    }
}

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[MAX_MSG];

    // Create socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("socket creation failed");
        exit(1);
    }

    // Set address and port number for the server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Connect to the server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connection failed");
        exit(1);
    }

    printf("Connected to server...\n");

    // Get message from user
    printf("Enter message: ");
    fgets(buffer, MAX_MSG, stdin);
    buffer[strlen(buffer) - 1] = '\0'; // remove newline character

    // Encrypt message
    encrypt_message(buffer);
    printf("Encrypted message: %s\n", buffer);

    // Send encrypted message to server
    send(client_fd, buffer, strlen(buffer), 0);

    // Close socket
    close(client_fd);

    return 0;
}