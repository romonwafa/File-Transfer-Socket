// File Transfer Sockets (server)
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
        fprintf(stderr, "Usage: %s <destination filename>\n", argv[0]);
        exit(1);
    }

    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    struct student_record buffer;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    // Initialize socket structure
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = 53385;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Bind the host address
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    // Listen for connections
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    // Accept connection
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");

    // Open file for writing
    int file_fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (file_fd < 0)
        error("ERROR opening file");

    // Receive and write data
    while (1)
    {
        bzero(&buffer, sizeof(buffer));

        // Receive data from client
        int n = read(newsockfd, &buffer, sizeof(buffer));
        if (n <= 0)
            break;

        // Write to file
        write(file_fd, buffer.payload, buffer.nbytes);

        // Check for end of file
        if (buffer.nbytes < sizeof(buffer.payload))
            break;
    }

    // Close connections and file
    close(newsockfd);
    close(sockfd);
    close(file_fd);

    printf("File received successfully.\n");

    return 0;
}

