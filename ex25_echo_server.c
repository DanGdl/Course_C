/*
compile: gcc -Wall -Wextra -Wpedantic ex25_echo_server.c -o ex25_echo_server   OR: gcc ex25_echo_server.c -o ex25_echo_server
run: ./ex25_echo_server
*/

#include <stdio.h>
#include <errno.h>      // errors
#include <stdlib.h>
#include <unistd.h>		// close
#include <string.h>     // error code to string
#include <stdint.h>
#include <stddef.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define LEN_ARRAY(x) (sizeof(x)/sizeof(x[0]))
#define LEN_STR(x) (sizeof(x)/sizeof(x[0])-1)

#define SIZE_BUFFER 1024
#define SOCK_PATH   "/tmp/example.socket"

int main(void) {
	const int reuse = 1;
	uint8_t buffer[SIZE_BUFFER] = { 0 };
	
    // open TCP socket
    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) {
        printf("Failed to create TCP socket: %s\n", strerror(errno));
        return -EXIT_FAILURE;
    }
    // set reuse option
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*) &reuse, sizeof(int)) == 1) {
        printf("Failed to reuse socket: %s\n", strerror(errno));
        close(server_fd);
        return -EXIT_FAILURE;
    }

    // bind socket
     struct sockaddr_un name;
    memset(&name, 0, sizeof(name));
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCK_PATH, sizeof(name.sun_path) - 1);

    const int size = offsetof(struct sockaddr_un, sun_path) + strlen(name.sun_path);
    int rc = bind(server_fd, (struct sockaddr*) &name, size);
    if (rc < 0) {
        printf("Failed to bind to socket: %s\n", strerror(errno));
        close(server_fd);
        return -EXIT_FAILURE;
    }
    rc = listen(server_fd, 10);
    if (rc < 0) {
        printf("Failed to listen on socket: %s\n", strerror(errno));
        close(server_fd);
        return -EXIT_FAILURE;
    }
    struct sockaddr_in client_addr = { 0 };
    unsigned int address_size = sizeof(client_addr);
    // while (1) {
    int connect_fd = accept(server_fd, (struct sockaddr*) &client_addr, &address_size);
    if (connect_fd < 0) {
        printf("Failed to receive connection\n");
        return -EXIT_FAILURE;
        // continue;
    }
    if (AF_INET == client_addr.sin_family) {
        struct sockaddr_in* pV4Addr = (struct sockaddr_in*) &client_addr;
        char str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &pV4Addr->sin_addr, str, INET_ADDRSTRLEN);
        printf("Handle request from ip4 %s: %d\n", str, pV4Addr->sin_port);
    }
    else if (AF_INET6 == client_addr.sin_family) {
        struct sockaddr_in6* pV6Addr = (struct sockaddr_in6*) &client_addr;
        char str[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &pV6Addr->sin6_addr, str, INET6_ADDRSTRLEN);
        printf("Handle request from ip6 %s: %d\n", str, pV6Addr->sin6_port);
    }
    const int received = read(connect_fd, buffer, LEN_ARRAY(buffer));
    printf("Received %d bytes: %s\n", received, buffer);

    const size_t to_send = strlen((char*) buffer);
    const int sent = write(connect_fd, buffer, to_send);
    printf("Sent %d bytes from %zu\n", sent, to_send);
    if (sent < 0 || (unsigned int) sent < to_send) {
        printf("Failed to send message to client: %s\n", strerror(errno));
    }
    memset(buffer, 0, sizeof(buffer));
    close(connect_fd);
    // }
    close(server_fd);
    server_fd = -1;
    unlink(SOCK_PATH);

    return EXIT_SUCCESS;
}
