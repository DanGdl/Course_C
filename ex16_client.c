/*
compile: gcc -Wall -Wextra -Wpedantic ex16_client.c -o ex16_client   OR: gcc ex16_client.c -o ex16_client
run: ./ex16_client
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

typedef struct sockaddr_storage InetAddress_t;

int main(void) {
    const char msg[] = "Hi server!";
    const int reuse = 1;

    uint8_t buffer[SIZE_BUFFER] = { 0 };
    struct sockaddr_in si = { 0 };
    int client_fd = -1;
    int received = -1;
    int sent = -1;

#if 1
    // open TCP socket
    client_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        printf("Failed to create TCP socket: %s\n", strerror(errno));
        return -EXIT_FAILURE;
    }

    // set reuse option
    if (setsockopt(client_fd, SOL_SOCKET, SO_REUSEADDR, (char*) &reuse, sizeof(int)) == 1) {
        printf("Failed to reuse socket: %s\n", strerror(errno));
        close(client_fd);
        return -EXIT_FAILURE;
    }

    // connect to remote socket
    
    // memset(&si, 0, sizeof(si));
    si.sin_family = PF_INET;
    si.sin_addr.s_addr = inet_addr(IP_SERVER);
    si.sin_port = htons(PORT_SERVER);
    int rc = connect(client_fd, (struct sockaddr*) &si, sizeof(si));
    if (rc != 0) {
        printf("Failed to connect client socket: %s\n", strerror(errno));
        close(client_fd);
        return -EXIT_FAILURE;
    }

    // send message
    sent = send(client_fd, msg, LEN_ARRAY(msg), 0);
    printf("Sent %d bytes from %lu\n", sent, LEN_ARRAY(msg));
    if (sent < 0 || (unsigned int) sent < LEN_ARRAY(msg)) {
        printf("Failed to send message to server: %s\n", strerror(errno));
    }
    
    // receive message
    received = recv(client_fd, buffer, LEN_ARRAY(buffer), 0);
    printf("Received %d bytes: %s\n", received, buffer);
    memset(buffer, 0, sizeof(buffer));

    close(client_fd);
    client_fd = -1;

    sleep(1);
#endif
    
    // open UDP socket
    client_fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (client_fd < 0) {
        printf("Failed to create UDP socket: %s\n", strerror(errno));
        return -EXIT_FAILURE;
    }

    // set reuse option
    if (setsockopt(client_fd, SOL_SOCKET, SO_REUSEADDR, (char*) &reuse, sizeof(int)) == 1) {
        printf("Failed to reuse socket: %s\n", strerror(errno));
        close(client_fd);
        return -EXIT_FAILURE;
    }

    // connect to remote socket
    // struct sockaddr_in si = { 0 };
    memset(&si, 0, sizeof(si));
    si.sin_family = PF_INET;
    si.sin_addr.s_addr = inet_addr(IP_SERVER);
    si.sin_port = htons(PORT_SERVER);

    // send message
    sent = sendto(client_fd, msg, LEN_ARRAY(msg), MSG_CONFIRM, (struct sockaddr*) &si, sizeof(si));
    printf("Sent %d bytes from %lu\n", sent, LEN_ARRAY(msg));
    if (sent < 0 || (unsigned int) sent < LEN_ARRAY(msg)) {
        printf("Failed to send message to server: %s\n", strerror(errno));
    }
    
    struct sockaddr_in address = { 0 };
    unsigned int addr_len = sizeof(address);
    // receive message
    received = recvfrom(client_fd, buffer, LEN_ARRAY(buffer), MSG_WAITALL, (struct sockaddr*) &address, &addr_len);
    if (received <= 0) {
        printf("Failed to receive data\n");
        // continue;
    }
    char ip_addr[128] = { 0 };
    if (AF_INET == address.sin_family) {
        struct sockaddr_in* pV4Addr = (struct sockaddr_in*) &address;
        inet_ntop(AF_INET, &pV4Addr->sin_addr, ip_addr, INET_ADDRSTRLEN);
        printf("Received %d bytes from %s:%d -> %s\n", received, ip_addr, pV4Addr->sin_port, buffer);
    }
    else if (AF_INET6 == address.sin_family) {
        struct sockaddr_in6* pV6Addr = (struct sockaddr_in6*) &address;
        inet_ntop(AF_INET6, &pV6Addr->sin6_addr, ip_addr, INET6_ADDRSTRLEN);
        printf("Received %d bytes from %s:%d -> %s\n", received, ip_addr, pV6Addr->sin6_port, buffer);
    }
    else {
        printf("Received %d bytes from unknown IP type -> %s\n", received, buffer);
    }
    memset(buffer, 0, sizeof(buffer));
    close(client_fd);
    client_fd = -1;

    return EXIT_SUCCESS;
}
