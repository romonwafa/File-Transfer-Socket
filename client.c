// File Transfer using Sockets (client)
// Author: Romon Wafa

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct student_record
{
    int nbytes;
    char payload[1024];
};

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

// Main Function
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <source filename>\n", argv[0]);
        exit(1);
    }

    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct student_record buffer;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    // Initialize socket structure
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = 53385;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);

    // Use server address 127.0.0.1
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
        error("ERROR converting IP address");

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    // Open file for reading
    int file_fd = open(argv[1], O_RDONLY);
    if (file_fd < 0)
        error("ERROR opening file");

    // Send data to server
    while (1)
    {
        bzero(&buffer, sizeof(buffer));

        // Read data from file
        buffer.nbytes = read(file_fd, buffer.payload, sizeof(buffer.payload));

        // Send data to server
        int n = write(sockfd, &buffer, sizeof(buffer));
        if (n <= 0)
            break;

        // Check for end of file
        if (buffer.nbytes < sizeof(buffer.payload))
            break;
    }

    // Close connections and file
    close(sockfd);
    close(file_fd);

    printf("File sent successfully.\n");

    return 0;
}

