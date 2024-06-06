#include "client.h"
#include <sys/time.h>
#include <poll.h>

/**
 * @brief Main function for the client.
 * This function establishes a connection to the server, sends a message, and receives the same message back.
 * @return int Exit status.
 */

static int socketfd;

int main() {
    struct sockaddr_in servaddr;
    char sendline[MAXLINE + 1];
    char recvline[MAXLINE + 1];

    // Register signal handler for SIGINT (Ctrl+C)
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        handle_err("signal");
    }

    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        handle_err("Socket creation error");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (connect(socketfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        handle_err("connect failed");
    }
    

    int bytes_received;
    struct pollfd pfd;
    pfd.fd = socketfd;
    pfd.events = POLLIN | POLLHUP | POLLERR; // Poll the events on socket to check connection status

    while (true) {

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

        int size = strlen(sendline) + 1;
        if (HEADER_SIZE != sizeof(size) || size <= 0) {
            fprintf(stdout, "invalid header or message size\r\n");
            continue;
        }
        fprintf(stdout, "Sent data of size: %d, ", size-1);
        fflush(stdout);

        int size_network = htonl(size);
        if (send(socketfd, &size_network, HEADER_SIZE, MSG_NOSIGNAL) < 0) {
            handle_err("send error");
        }
        if (send(socketfd, sendline, size, MSG_NOSIGNAL) < 0) {
            handle_err("send error");
        }  


        int recv_size_network = 0;
        if((bytes_received = recv(socketfd, &recv_size_network, HEADER_SIZE, 0)) <= 0){
        if (bytes_received == 0) {
                fprintf(stdout, "Server closed the connection\r\n");
            } else if (bytes_received < 0) {
                handle_err("recv failed");
            }
            fflush(stdout);
            break;
        } 
        int recv_size = ntohl(recv_size_network);
        memset(recvline, 0, MAXLINE);
        if((bytes_received = recv(socketfd, recvline, recv_size, 0)) <= 0){
        if (bytes_received == 0) {
                fprintf(stdout, "Server closed the connection\r\n");
            } else if (bytes_received < 0) {
                handle_err("recv failed");
            }
            fflush(stdout);
            break;
        } 
        fprintf(stdout, "Received size: %d, ", recv_size-1);
        fprintf(stdout, "server echo: %s\r\n", recvline);
        fflush(stdout);

        int poll_result = poll(&pfd, 1, 1000);
        if (poll_result < 0) {
            handle_err("poll error");
        } else if (pfd.revents & (POLLHUP | POLLERR)) {
                fprintf(stdout, "Connection closed by server\r\n");
                break;
        }
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
