/**
 * @file client.c
 * @brief Simple client code for POSIX-compliant operating systems that receives back the message sent to the server.
 *
 * @details This client code sends a message to a server and receives the same message back.
 * A static buffer of 4KB is used to take input from the user and send it back.
 */

#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdarg.h>
#include <signal.h>
#include <errno.h>

#define SERVER_PORT 18000
#define MAXLINE 4096
#define HEADER_SIZE 4

/**
 * @brief Function to handle errors.
 *
 * This function prints the error message and exits the program.
 * 
 * @param format Format string for the error message.
 */

void handle_err(const char* format, ...);

/**
 * @brief Signal handler for SIGINT (Ctrl+C).
 *
 * This function closes the socket and exits the program when a SIGINT signal is received.
 * 
 * @param signum Signal number.
 */

void sigint_handler(int signum);

#endif /* __CLIENT_H__ */
