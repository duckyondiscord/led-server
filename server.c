#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>

#define SA struct sockaddr

int sockfd;

void handle_sigint(int sig) {
    printf("SIGINT received, terminating gracefully.");
    close(sockfd);
    exit(0);
}

int main(int argc, char* argv[])
{
    struct sockaddr_in servaddr = {};

    setbuf(stdout, NULL);
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }

    if (argc < 2) {
        printf("not enough arguments, exiting.\n");
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    // Binding newly created socket to given IP and port
    if (bind(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0)
        exit(0);
    // Now server is ready to listen and verification

    // Register signal handler for SIGINT
    signal(SIGINT, handle_sigint);

    listen(sockfd, 1);
    printf("Server listening on port %d\n", port);
    while (1) {
        struct sockaddr_in client_addr;
        int client_size = sizeof(client_addr);
        int client_sock = accept(sockfd, (SA*)&client_addr, &client_size);
        if (client_sock == -1) {
            printf("Error accepting connection...\n");
            continue; // Continue to the next iteration of the loop
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);

        char* response = "\nRequest acknowledged\n";
        char conn[128];
        memset(conn, 0, sizeof(conn));
        if (recv(client_sock, conn, sizeof(conn), 0) > 0) {
            send(client_sock, response, strlen(response), 0);
            if (strcmp(conn, "test") == 0) {
                printf("Request from %s succeeded\n", client_ip);
                system("mpremote reset");
            }
            else
                printf("Request from %s failed\n", client_ip);
        }

        close(client_sock); // Close the client socket
    }
    close(sockfd); // Close the server socket
}

