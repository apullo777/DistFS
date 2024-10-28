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
    char filename[100] = {0};

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

    printf("File transfer server listening on port %d...\n", PORT);

    // 4. Accept incoming connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // 5. Receive file name from client
    read(new_socket, filename, sizeof(filename));
    printf("Receiving file: %s\n", filename);

    // 6. Open file to write
    int file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (file_fd < 0) {
        perror("File open failed");
        exit(EXIT_FAILURE);
    }

    // 7. Receive file data in chunks and write to the file
    int bytes_received;
    while ((bytes_received = read(new_socket, buffer, BUFFER_SIZE)) > 0) {
        write(file_fd, buffer, bytes_received);
    }

    printf("File transfer completed.\n");

    // 8. Close file and sockets
    close(file_fd);
    close(new_socket);
    close(server_fd);

    return 0;
}