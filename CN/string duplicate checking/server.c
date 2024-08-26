#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_SENTENCE_LENGTH 1024

int remove_duplicates(char *sentence, char *unique_sentence) {
    int i, j, k;
    int unique_word_count = 0;

    for (i = 0; sentence[i] != '\0'; i++) {
        int is_duplicate = 0;
        for (j = 0; j < unique_word_count; j++) {
            if (isspace(sentence[i]) || isspace(unique_sentence[j * MAX_SENTENCE_LENGTH + k])) {
                continue;
            }
            if (sentence[i] == unique_sentence[j * MAX_SENTENCE_LENGTH + k]) {
                k++;
                continue;
            }
            if (sentence[i] != unique_sentence[j * MAX_SENTENCE_LENGTH + k]) {
                is_duplicate = 1;
                break;
            }
        }
        if (!is_duplicate) {
            strcpy(unique_sentence + unique_word_count * MAX_SENTENCE_LENGTH, sentence + i);
            unique_word_count++;
            while (!isspace(sentence[i]) && sentence[i] != '\0') {
                i++;
            }
        }
    }

    return unique_word_count;
}

int main() {
    int server_socket, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // Forcefully binding socket to the port 8080
    if (bind(server_socket, (struct sockaddr *)&address, addrlen) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port 8080...\n");

    while (1) {
        new_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (new_socket < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        char sentence[MAX_SENTENCE_LENGTH] = {0};
        char unique_sentence[MAX_SENTENCE_LENGTH * MAX_SENTENCE_LENGTH] = {0};

        valread = read(new_socket, sentence, MAX_SENTENCE_LENGTH);
        if (valread == 0) {
            printf("Client disconnected\n");
            close(new_socket);
            continue;
        }

        if (strcmp(sentence, "Stop") == 0) {
            printf("Server stopping...\n");
            close(new_socket);
            break;
        }

        int unique_word_count = remove_duplicates(sentence, unique_sentence);
        send(new_socket, unique_sentence, unique_word_count * MAX_SENTENCE_LENGTH, 0);

        printf("Sent unique sentence to client: %s\n", unique_sentence);
    }

    return 0;
}