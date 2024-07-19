/*
compile: gcc -Wall -Wextra -Wpedantic ex16_echo_server.c -o ex16_server   OR: gcc ex16_echo_server.c -o ex16_server
run: ./ex16_server
*/

#include <stdio.h>
#include <errno.h>      // errors
#include <stdlib.h>
#include <stdlib.h>
#include <unistd.h>		// close
#include <string.h>     // error code to string
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define LEN_ARRAY(x) (sizeof(x)/sizeof(x[0]))
#define LEN_STR(x) (sizeof(x)/sizeof(x[0])-1)

#define PORT_SERVER 2000
#define IP_SERVER   "127.0.0.1"
#define SIZE_BUFFER 1024


int main(void) {
	const int reuse = 1;

	uint8_t buffer[SIZE_BUFFER] = { 0 };
	struct sockaddr_in name = { 0 };
	size_t to_send = 0;
	int server_fd = -1;
	int received = -1;
	int sent = -1;
	int rc = -1;

#if 1
    // open TCP socket
    server_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        printf("Failed to create TCP socket: %s\n", strerror(errno));
        return -EXIT_FAILURE;
    }
    // bind params
    name.sin_family = PF_INET;
    name.sin_port = htons(PORT_SERVER);
    name.sin_addr.s_addr = htonl(INADDR_ANY);

    // set reuse option
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*) &reuse, sizeof(int)) == 1) {
        printf("Failed to reuse socket: %s\n", strerror(errno));
        close(server_fd);
        return -EXIT_FAILURE;
    }

    // bind socket
    rc = bind(server_fd, (struct sockaddr*) &name, sizeof(name));
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
    received = recv(connect_fd, buffer, LEN_ARRAY(buffer), MSG_DONTWAIT);
    printf("Received %d bytes: %s\n", received, buffer);

    to_send = strlen((char*) buffer);
    sent = send(connect_fd, buffer, to_send, 0);
    printf("Sent %d bytes from %zu\n", sent, to_send);
    if (sent < 0 || (unsigned int) sent < to_send) {
        printf("Failed to send message to client: %s\n", strerror(errno));
    }
    memset(buffer, 0, sizeof(buffer));
    close(connect_fd);
    // }
    close(server_fd);
    server_fd = -1;
#endif

    puts("UDP server");
    // open UDP socket
    server_fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (server_fd < 0) {
        printf("Failed to create UDP socket: %s\n", strerror(errno));
        return -EXIT_FAILURE;
    }
    // set reuse option
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*) &reuse, sizeof(int)) == 1) {
        printf("Failed to reuse socket: %s\n", strerror(errno));
        close(server_fd);
        return -EXIT_FAILURE;
    }

    memset(&name, 0, sizeof(name));
	name.sin_family = PF_INET;
	name.sin_port = (in_port_t) htons(PORT_SERVER);
	name.sin_addr.s_addr = htonl(INADDR_ANY);

	// bind socket
	rc = bind(server_fd, (struct sockaddr*) &name, sizeof(name));
	if (rc == -1) {
		printf("Failed to bind to socket: %s\n", strerror(errno));
		close(server_fd);
		return -1;
	}

	struct sockaddr_storage address = { 0 };
	socklen_t addr_len = sizeof(address);
    received = recvfrom(server_fd, buffer, LEN_ARRAY(buffer), 0 /*MSG_WAITFORONE*/, (struct sockaddr*) &address, &addr_len);
    if (received <= 0) {
        printf("Failed to receive data\n");
        // continue;
    }
    if (AF_INET == address.ss_family) {
        struct sockaddr_in* pV4Addr = (struct sockaddr_in*) &address;
        char ip_addr[INET_ADDRSTRLEN] = { 0 };
        inet_ntop(AF_INET, &pV4Addr->sin_addr, ip_addr, INET_ADDRSTRLEN);
        printf("Received %d bytes from %s:%d -> %s\n", received, ip_addr, pV4Addr->sin_port, buffer);
    }
    else if (AF_INET6 == address.ss_family) {
        struct sockaddr_in6* pV6Addr = (struct sockaddr_in6*) &address;
        char ip_addr[INET6_ADDRSTRLEN] = { 0 };
        inet_ntop(AF_INET6, &pV6Addr->sin6_addr, ip_addr, INET6_ADDRSTRLEN);
        printf("Received %d bytes from %s:%d -> %s\n", received, ip_addr, pV6Addr->sin6_port, buffer);
    }
    else {
        printf("Received %d bytes from unknown IP type -> %s\n", received, buffer);
    }
    to_send = strlen((char*) buffer);
    // send message
    sent = sendto(server_fd, buffer, to_send, MSG_CONFIRM, (struct sockaddr*) &address, sizeof(address));
    printf("Sent %d bytes from %zu\n", sent, to_send);
    if (sent < 0 || (unsigned int) sent < to_send) {
        printf("Failed to send message to client: %s\n", strerror(errno));
    }
    
    memset(buffer, 0, sizeof(buffer));
    close(server_fd);
    server_fd = -1;

    return EXIT_SUCCESS;
}
