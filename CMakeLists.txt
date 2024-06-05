cmake_minimum_required(VERSION 3.10)
project(SimpleEchoClientServer C)

set(CMAKE_C_STANDARD 11)

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# Client
add_executable(client client/client.c)
target_include_directories(client PUBLIC client)

# Server
add_executable(server server/server.c)
target_include_directories(server PUBLIC server)

# Custom target for building
add_custom_target(build
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
    COMMENT "Building SimpleEchoClientServer project"
)

# Custom target for cleaning
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/../bin
    COMMENT "Cleaning SimpleEchoClientServer project"
)

# Custom target for running server
add_custom_target(run-server
    COMMAND ${CMAKE_BINARY_DIR}/../bin/server
    COMMENT "Running server"
)

# Custom target for running server and client
add_custom_target(run-client
    COMMAND ${CMAKE_BINARY_DIR}/../bin/client
    COMMENT "Runningclient"
)