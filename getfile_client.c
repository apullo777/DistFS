#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char *filename = "testfile.txt"; // The file to request

    // 1. Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    // 2. Define server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Invalid address / Address not supported\n");
        return -1;
    }

    // 3. Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection failed\n");
        return -1;
    }

    // 4. Send GET request with file name
    send(sock, filename, strlen(filename) + 1, 0); // +1 to include null terminator
    printf("GET request sent for file: %s\n", filename);

    // 5. Receive response from server
    int valread = read(sock, buffer, BUFFER_SIZE);
    printf("Server response: %s", buffer);

    // 6. Check for success or error in response
    if (strncmp(buffer, "SUCCESS", 7) == 0) {
        // Server found the file, start receiving file content
        int file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (file_fd < 0) {
            perror("File open failed");
            return -1;
        }

        // Clear buffer and start receiving file content
        memset(buffer, 0, BUFFER_SIZE);
        while ((valread = read(sock, buffer, BUFFER_SIZE)) > 0) {
            write(file_fd, buffer, valread);
        }

        printf("File download completed.\n");
        close(file_fd);
    } else {
        // Server did not find the file
        printf("File not found on server.\n");
    }

    // 7. Close the socket
    close(sock);

    return 0;
}