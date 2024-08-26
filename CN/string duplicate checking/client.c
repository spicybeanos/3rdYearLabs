#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAXLINE 1024

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[MAXLINE];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(1);
    }

    // Set address and port number for the server
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect failed");
        exit(1);
    }

    printf("Connected to server...\n");

    while (1) {
        printf("Enter a sentence (or 'Stop' to quit): ");
        fgets(buffer, MAXLINE, stdin);
        buffer[strlen(buffer) - 1] = '\0'; // remove newline character

        // Send message to server
        write(sockfd, buffer, strlen(buffer));

        // Check for "Stop" message
        if (strcmp(buffer, "Stop") == 0) {
            break;
        }

        // Receive response from server
        read(sockfd, buffer, MAXLINE);
        printf("Received response from server: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}