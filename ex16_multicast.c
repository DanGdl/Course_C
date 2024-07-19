/*
compile: gcc -Wall -Wextra -Wpedantic ex16_multicast.c -o ex16_multicast   OR: gcc ex16_multicast.c -o ex16_multicast
run: ./ex16_multicast
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

    #include <sys/types.h>
    #include <time.h>

#define LEN_ARRAY(x)    (sizeof(x)/sizeof(x[0]))
#define LEN_STR(x)      (sizeof(x)/sizeof(x[0])-1)

#define MAX(a, b)       ((a) > (b) ? (a) : (b))
#define MIN(a, b)       ((a) < (b) ? (a) : (b))

#define TYPE_SENDER     "sender"
#define TYPE_RECEIVER   "receiver"

#define PORT_SERVER     2000
#define IP_SERVER       "224.1.1.1"
#define SIZE_BUFFER     1024


int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Provide socket type: %s or %s\n", TYPE_SENDER, TYPE_RECEIVER);
        return EXIT_SUCCESS;
    }
    if (memcmp(argv[1], TYPE_RECEIVER, MIN(sizeof(TYPE_RECEIVER), strlen(argv[1]))) == 0) {
        int fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (fd < 0) {
            printf("Failed to open socket: %s\n", strerror(errno));
            return 1;
        }

        // allow multiple sockets to use the same PORT number
        u_int yes = 1;
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof(yes)) < 0){
           printf("Failed to make socket reusable: %s\n", strerror(errno));
           return 1;
        }

        struct sockaddr_in addr = { 0 };
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(PORT_SERVER);

        // bind to receive address
        if (bind(fd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
            printf("Failed to bind socket to port: %s\n", strerror(errno));
            return 1;
        }

        // join to multicast group
        struct ip_mreq mreq = { 0 };
        mreq.imr_multiaddr.s_addr = inet_addr(IP_SERVER);
        mreq.imr_interface.s_addr = htonl(INADDR_ANY);
        if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq)) < 0){
            printf("Failed to join to multicast group: %s\n", strerror(errno));
            return 1;
        }

        short cnt = 0;
        char msgbuf[SIZE_BUFFER] = { 0 };
        memset(&addr, 0, sizeof(addr));
        socklen_t addrlen = sizeof(addr);
        while (cnt < 10) {
            int nbytes = recvfrom(fd, msgbuf, SIZE_BUFFER, 0, (struct sockaddr *) &addr, &addrlen);
            if (nbytes < 0) {
                printf("Failed to receive: %s\n", strerror(errno));
                close(fd);
                return 1;
            }
            msgbuf[nbytes] = '\0';
            puts(msgbuf);
            cnt++;
        }
        close(fd);
        fd = -1;
    }
    else if (memcmp(argv[1], TYPE_SENDER, MIN(sizeof(TYPE_SENDER), strlen(argv[1]))) == 0) {
        int fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (fd < 0) {
            printf("Failed to open socket: %s\n", strerror(errno));
            return 1;
        }

        // allow multiple sockets to use the same PORT number
        u_int yes = 1;
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof(yes)) < 0){
           printf("Failed to make socket reusable: %s\n", strerror(errno));
           return 1;
        }

        short cnt = 0;
        struct sockaddr_in addr = { 0 };
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(IP_SERVER);
        addr.sin_port = htons(PORT_SERVER);

        const char* template = "Message %d";
        char buffer[SIZE_BUFFER] = { 0 };
        while (cnt < 10) {
            int rc = sprintf(buffer, template, cnt);
            if (rc < 0) {
                printf("Failed to prepare message: %s\n", strerror(errno));
            }
            int nbytes = sendto(fd, buffer, strlen(buffer), 0, (struct sockaddr*) &addr, sizeof(addr));
            if (nbytes < 0) {
                printf("Failed to send message: %s\n", strerror(errno));
                return 1;
            }
            sleep(1);
            cnt++;
        }
        close(fd);
        fd = -1;
    }
    return EXIT_SUCCESS;
}
