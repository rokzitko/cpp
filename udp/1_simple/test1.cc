#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in servaddr, cliaddr;

    // Create socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Zero out the server address structure
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Fill server information
    servaddr.sin_family = AF_INET;  // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    socklen_t len;
    ssize_t n;

    len = sizeof(cliaddr);  // Length of client address

    std::cout << "Waiting for UDP packets on port " << PORT << "...\n";

    // Receive data from client
    n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&cliaddr, &len);
    if (n < 0) {
        perror("Receive failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    buffer[n] = '\0';  // Null-terminate the received data
    std::cout << "Received packet: " << buffer << "\n";

    // Close the socket
    close(sockfd);
    return 0;
}

