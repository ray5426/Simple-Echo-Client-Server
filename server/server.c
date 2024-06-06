#include "server.h"

/**
 * @brief Main function for the server.
 * This function sets up a server that listens for a client connection, receives a message, and sends the same message back.
 * @return int Exit status.
 */

int main() {
    int listenfd, connfd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAXLINE + 1] = {0}; // plus 1 for null terminator

    // Register signal handler for SIGINT (Ctrl+C)
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        handle_err("signal");
    }

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        handle_err("socket error");
    }
    bzero(&address, addrlen);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(SERVER_PORT);
    if (bind(listenfd, (struct sockaddr *)&address, addrlen) < 0) {
        handle_err("bind failed");
    }

    if (listen(listenfd, 1) < 0) {      // Only one connection will be accepted at a time, rest will be rejected 
        handle_err("listen failed");
    }
 
    while (true) {
        int size_network; 
        struct sockaddr_in addr;
        socklen_t addr_len;

        fprintf(stdout, "Waiting for new connection on port %d\r\n", SERVER_PORT);
        fflush(stdout);
        connfd = accept(listenfd, (struct sockaddr*)&addr, &addr_len);

        /**
         * Once a connection is accepted, the following statements are executed.
         * If the connection is lost, the program waits for another connection.
         */
        while (true) {
            memset(buffer, 0, MAXLINE+1);
            size_network = 0;
            int bytes_received;

            if ((bytes_received = recv(connfd, &size_network, HEADER_SIZE, 0)) <= 0) {
                if (bytes_received == 0) {
                    fprintf(stdout, "Client closed the connection\r\n");
                } else if (bytes_received < 0) {
                    handle_err("recv failed");
                }
                fflush(stdout);
                close(connfd);
                break; 
            }
            int size = ntohl(size_network);

            if (size > MAXLINE + 1) {
                int error_length = sprintf(buffer,"Invalid header");
                int error_length_network = htonl(error_length+1);
                fprintf(stdout, "%s\r\n",buffer);
                send(connfd, &error_length_network, HEADER_SIZE, 0);
                send(connfd, buffer, error_length+1, 0);
                close(connfd);
                break;
            }
            fprintf(stdout, "Header value: %d ", size);

            if ((bytes_received = recv(connfd, buffer, size, 0)) <= 0) {
                if (bytes_received == 0) {
                    fprintf(stdout, "Client closed the connection\r\n");
                } else if (bytes_received < 0) {
                    handle_err("recv failed");
                }
                fflush(stdout);
                close(connfd);
                break; 
            }
            buffer[MAXLINE] = '\0'; // Ensure buffer is null-terminated
            fprintf(stdout, "message: %s\r\n", buffer);
            fflush(stdout);

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
