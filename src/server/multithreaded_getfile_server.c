#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void *handle_client(void *socket_desc) {
    int client_sock = *(int*)socket_desc;
    free(socket_desc);
    
    char buffer[BUFFER_SIZE] = {0};
    char filename[100];

    // 1. Receive the file name from the client
    if (read(client_sock, filename, sizeof(filename)) <= 0) {
        perror("Failed to read file name");
        close(client_sock);
        return NULL;
    }
    printf("Client requested file: %s\n", filename);

    // 2. Check if the file exists
    int file_fd = open(filename, O_RDONLY);
    if (file_fd < 0) {
        // File does not exist; send error message
        char *error_msg = "ERROR: File not found\n";
        send(client_sock, error_msg, strlen(error_msg), 0);
        printf("File not found. Sent error message to client.\n");
    } else {
        // File exists; send success message
        char *success_msg = "SUCCESS: File found\n";
        send(client_sock, success_msg, strlen(success_msg), 0);

        // 3. Send the file in chunks
        int bytes_read;
        while ((bytes_read = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
            send(client_sock, buffer, bytes_read, 0);
        }
        printf("File transfer to client completed.\n");

        // Close the file descriptor
        close(file_fd);
    }

    // 4. Close client socket
    close(client_sock);
    return NULL;
}

int main() {
    int server_fd, *new_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // 1. Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 3. Listen for incoming connections
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Multithreaded GETFILE server listening on port %d...\n", PORT);

    // 4. Accept connections and create threads for each client
    while ((new_sock = malloc(sizeof(int)), *new_sock = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen), *new_sock >= 0)) {
        printf("Connection accepted.\n");

        // Create a new thread for each client
        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, handle_client, (void*) new_sock) < 0) {
            perror("Could not create thread");
            free(new_sock);
            continue;
        }

        // Detach thread to free resources after it finishes
        pthread_detach(client_thread);
    }

    if (*new_sock < 0) {
        perror("Accept failed");
    }

    // Close the server socket
    close(server_fd);
    return 0;
}