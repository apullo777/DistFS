#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

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
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("GETFILE server listening on port %d...\n", PORT);

    // 4. Accept incoming connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // 5. Receive the file name from the client
    read(new_socket, buffer, BUFFER_SIZE);
    printf("Requested file: %s\n", buffer);

    // 6. Check if the file exists
    int file_fd = open(buffer, O_RDONLY);
    if (file_fd < 0) {
        // File does not exist; send error message
        char *error_msg = "ERROR: File not found\n";
        send(new_socket, error_msg, strlen(error_msg), 0);
        printf("File not found. Sent error message to client.\n");
    } else {
        // File exists; send success message
        char *success_msg = "SUCCESS: File found\n";
        send(new_socket, success_msg, strlen(success_msg), 0);

        // 7. Send the file in chunks
        int bytes_read;
        while ((bytes_read = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
            send(new_socket, buffer, bytes_read, 0);
        }
        printf("File transfer completed.\n");

        // Close the file descriptor
        close(file_fd);
    }

    // 8. Close sockets
    close(new_socket);
    close(server_fd);

    return 0;
}