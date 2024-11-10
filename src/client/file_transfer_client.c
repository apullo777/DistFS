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
    char *filename = "testfile.txt"; // The file you want to send

    // 1. Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    // 2. Set up server address
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

    // 4. Send file name to server
    send(sock, filename, strlen(filename) + 1, 0); // +1 to include the null terminator

    // 5. Open file to read
    int file_fd = open(filename, O_RDONLY);
    if (file_fd < 0) {
        perror("File open failed");
        return -1;
    }

    // 6. Read file content and send it in chunks
    int bytes_read;
    while ((bytes_read = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
        send(sock, buffer, bytes_read, 0);
    }

    printf("File transfer completed.\n");

    // 7. Close file and socket
    close(file_fd);
    close(sock);

    return 0;
}