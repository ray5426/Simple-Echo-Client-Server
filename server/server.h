/**
 * @file server.c
 * @brief Simple server code for POSIX-compliant operating systems that sends back the message received from the client.
 *
 * @details
 * This server code accepts a connection from a client, receives a message, and sends the same message back.
 * The server accepts only one client at a time and no multithreading is done.
 * A static buffer of 4KB is used to take input from the user and send it back.
 */

#ifndef __SERVER_H__
#define __SERVER_H__

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
 * This function prints a message and exits the program when a SIGINT signal is received.
 * 
 * @param signum Signal number.
 */

void sigint_handler(int signum);

#endif /* __SERVER_H__ */
