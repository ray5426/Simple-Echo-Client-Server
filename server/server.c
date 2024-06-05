#include "server.h"

/**
 * @brief Main function for the server.
 *
 * This function sets up a server that listens for a client connection, receives a message, and sends the same message back.
 * 
 * @return int Exit status.
 */

int main() {
    int listenfd, connfd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAXLINE] = {0};

    // Register signal handler for SIGINT (Ctrl+C)
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        handle_err("signal");
    }

    // Create a socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        handle_err("socket error");
    }

    // Initialize server address structure
    bzero(&address, addrlen);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(SERVER_PORT);

    // Bind the socket to the address and port
    if (bind(listenfd, (struct sockaddr *)&address, addrlen) < 0) {
        handle_err("bind failed");
    }

    // Start listening for incoming connections. Only one connection will be accepted at a time, rest will be rejected 
    if (listen(listenfd, 1) < 0) {
        handle_err("listen failed");
    }
 
    while (true) {
        int size_network; 
        struct sockaddr_in addr;
        socklen_t addr_len;

        // Accept incoming connection
        fprintf(stdout, "Waiting for new connection on port %d\r\n", SERVER_PORT);
        fflush(stdout);
        connfd = accept(listenfd, (struct sockaddr*)&addr, &addr_len);

        /**
         * Once a connection is accepted, the following statements are executed.
         * If the connection is lost, the program waits for another connection.
         */
        while (true) {
            memset(buffer, 0, MAXLINE);
            size_network = 0;
            int bytes_received;

            // Receive the message size
            if ((bytes_received = recv(connfd, &size_network, HEADER_SIZE, 0)) <= 0) {
                if (bytes_received == 0) {
                    fprintf(stdout, "Client closed the connection\r\n");
                } else if (bytes_received < 0) {
                    handle_err("recv failed");
                }
                fflush(stdout);
                break; 
            }

            // Convert network byte order to host byte order
            int size = ntohl(size_network);
            if (sizeof(size) != HEADER_SIZE) {
                fprintf(stdout, "Invalid header size\r\n");
                continue;
            }
            fprintf(stdout, "Header value: %d ", size);

            // Receive the message
            if ((bytes_received = recv(connfd, buffer, size, 0)) <= 0) {
                if (bytes_received == 0) {
                    fprintf(stdout, "Client closed the connection\r\n");
                } else if (bytes_received < 0) {
                    handle_err("recv failed");
                }
                fflush(stdout);
                break; 
            }
            fprintf(stdout, "message: %s\r\n", buffer);
            fflush(stdout);

            // Send the response back to the client
            send(connfd, &size_network, HEADER_SIZE, 0);
            send(connfd, buffer, size, 0);
        }
        fflush(stdout);
    }
    return 0;
}

void handle_err(const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    int errno_save = errno;
    if (errno_save != 0) {
        fprintf(stderr, ": %s", strerror(errno));
    }
    fprintf(stderr, "\r\n");
    fflush(stderr);
    va_end(ap);
    exit(EXIT_FAILURE);
}

void sigint_handler(int signum) {
    fprintf(stdout, "Ctrl+C signal received. Exiting...\r\n");
    fflush(stdout);
    exit(signum);
}
