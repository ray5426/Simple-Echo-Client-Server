# Simple-Echo-Client-Server
This repository contains a simple echo client-server application implemented for POSIX-compliant operating systems. The project demonstrates basic socket programming concepts where a client sends a message to a server, and the server echoes the message back to the client. This example is useful for understanding fundamental network operations such as creating sockets, connecting to a server, sending and receiving data, and handling signals and errors. This project serves as a practical introduction to socket programming and can be used as a foundation for more advanced network applications.

Features:

#Client: Connects to the server, sends a user-provided message, and receives the same message back from the server.

#Server: Accepts a single connection from the client, receives the message, and sends the same message back.

#Error Handling: Implements basic error handling for system calls to ensure robustness.

#Signal Handling: Handles SIGINT (Ctrl+C) to gracefully shut down both the client and server.
