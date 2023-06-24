#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1" // Replace with server IP
#define SERVER_PORT 8000      // Replace with server port

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char *message = "test";
    char server_response[128];

    // Create a socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(server_address.sin_addr)) <= 0) {
        perror("Invalid address or address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Send the message to the server
    if (send(client_socket, message, strlen(message), 0) < 0) {
        perror("Message sending failed");
        exit(EXIT_FAILURE);
    }

    // Receive response from the server
    if (recv(client_socket, server_response, sizeof(server_response), 0) < 0) {
        perror("Response receiving failed");
        exit(EXIT_FAILURE);
    }
    printf("Server response: %s\n", server_response);
    // Close the socket
    close(client_socket);

    return 0;
}

