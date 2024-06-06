# Simple Echo Client-Server

This project demonstrates a simple client-server application for POSIX-compliant operating systems. The client sends a message to the server, and the server echoes the message back to the client.

## Why This Project?

This example code was written to demonstrate basic socket programming concepts in a POSIX environment. It is a straightforward example that highlights the fundamental operations of creating sockets, binding, listening, connecting, sending, and receiving data over a network. It serves as a practical introduction to socket programming and can be used as a foundation for more advanced network applications.

## Features

- **Client**: Connects to the server, sends a message, and receives the same message back,`MAX MESSAGE SIZE is 4096 Bytes`.
- **Server**: Accepts a connection from the client, receives a message, and sends the same message back.
- **Error Handling**: Basic error handling is implemented for system calls and message size.
- **Signal Handling**: Handles `SIGINT` (Ctrl+C) to gracefully shut down the client and server.

## Building and Running

To build the project, follow these steps:

1. Create a build directory in the root directory of the project:
   ```bash
   mkdir build
2. Navigate to the build directory:
   ```bash
   cd build
3. Run CMake to generate build files:
   ```bash
   cmake ..
4. Build the executables:
   ```bash
   make build
5. To run the server, use the following command:
   ```bash
   make run-server
6. To run the server, use the following command:
   ```bash
   make run-client
