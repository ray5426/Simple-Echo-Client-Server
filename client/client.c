#include "client.h"

/**
 * @brief Main function for the client.
 *
 * This function establishes a connection to the server, sends a message, and receives the same message back.
 * 
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return int Exit status.
 */

static int socketfd;

int main(int argc, char** argv) {
    struct sockaddr_in servaddr;
    char sendline[MAXLINE + 1];
    char recvline[MAXLINE + 1];

    // Register signal handler for SIGINT (Ctrl+C)
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        handle_err("signal");
    }

    // Create a socket
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        handle_err("Socket creation error");
    }
    // Initialize server address structure
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // Connect to the server
    if (connect(socketfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        handle_err("connect failed");
    }

    // Main loop for sending and receiving messages
    while (true) {
        // Get message from user 
        memset(sendline, 0, sizeof(sendline));
        fprintf(stdout, "Enter message (%d char at max), Ctrl+C to quit: ", MAXLINE);
        fflush(stdout);
        fgets(sendline, MAXLINE + 1, stdin);

        /**
         * Trim excess characters if input exceeds maximum length,
         * @sendline[MAXLINE-1] != '\n' is checked because fgets() reads the newline character as well.
         */
        if (strlen(sendline) >= MAXLINE && sendline[MAXLINE - 1] != '\n') {
            sendline[MAXLINE] = '\0';
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
        } else {
            sendline[strlen(sendline) - 1] = '\0';
        }
        fflush(stdout);

        // Get size of the message
        int size = strlen(sendline);
        fprintf(stdout, "Header value: %d ", size);
        fflush(stdout);

        // Validate message size
        if (HEADER_SIZE != sizeof(size) || size <= 0) {
            fprintf(stdout, "invalid header or message size\r\n");
            continue;
        }

        // Convert message size to network byte order
        int size_network = htonl(size);

        // Send message size to the server
        if (send(socketfd, &size_network, HEADER_SIZE, MSG_NOSIGNAL) < 0) {
            handle_err("send error");
        }

        // Send the message to the server
        if (send(socketfd, sendline, size, MSG_NOSIGNAL) < 0) {
            handle_err("send error");
        }       

        // Receive response from the server
        int recv_size_network = 0;
        recv(socketfd, &recv_size_network, HEADER_SIZE, 0);
        int recv_size = ntohl(recv_size_network);

        // Receive the echoed message from the server
        memset(recvline, 0, MAXLINE);
        recv(socketfd, recvline, recv_size, 0);
        fprintf(stdout, "server echo: %s\r\n", recvline);
        fflush(stdout);
    }
    fflush(stdout);

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
    printf("Ctrl+C signal received. Exiting...\r\n");
    close(socketfd);
    exit(signum);
}
