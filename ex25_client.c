/*
compile: gcc -Wall -Wextra -Wpedantic ex25_client.c -o ex25_client   OR: gcc ex25_client.c -o ex25_client
run: ./ex25_client
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
    const char msg[] = "Hi server!";
    const int reuse = 1;

    uint8_t buffer[SIZE_BUFFER] = { 0 };
    
    // open TCP socket
    int client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
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
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path) - 1);
    const int size = offsetof(struct sockaddr_un, sun_path) + strlen(addr.sun_path);
    int rc = connect(client_fd, (struct sockaddr*) &addr, size);
    if (rc != 0) {
        printf("Failed to connect client socket: %s\n", strerror(errno));
        close(client_fd);
        return -EXIT_FAILURE;
    }

    // send message
    const int sent = write(client_fd, msg, LEN_ARRAY(msg));
    printf("Sent %d bytes from %lu\n", sent, LEN_ARRAY(msg));
    if (sent < 0 || (unsigned int) sent < LEN_ARRAY(msg)) {
        printf("Failed to send message to server: %s\n", strerror(errno));
    }
    
    // receive message
    const int received = read(client_fd, buffer, LEN_ARRAY(buffer));
    printf("Received %d bytes: %s\n", received, buffer);
    memset(buffer, 0, sizeof(buffer));

    close(client_fd);
    client_fd = -1;

    return EXIT_SUCCESS;
}
