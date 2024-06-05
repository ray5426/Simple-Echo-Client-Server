cmake_minimum_required(VERSION 3.10)
project(SimpleEchoClientServer C)

set(CMAKE_C_STANDARD 11)

# Client
add_executable(client client/client.c)
target_include_directories(client PUBLIC client)

# Server
add_executable(server server/server.c)
target_include_directories(server PUBLIC server)
