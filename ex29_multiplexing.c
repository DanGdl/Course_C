/*
compile: gcc -Wall -Wextra -Wpedantic ex29_multiplexing.c -o ex29   OR: gcc ex29_multiplexing.c -o ex29
run: ./ex29
*/

//#define SELECT
//#define POLL
#define EPOLL

#include <stdio.h>
#include <errno.h>      // errors
#include <stdlib.h>
#include <unistd.h>     // close
#include <string.h>     // error code to string
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifdef SELECT
// https://man7.org/linux/man-pages/man2/select.2.html
#include <sys/select.h>

#elif defined(POLL)
// https://man7.org/linux/man-pages/man2/poll.2.html
#include <poll.h>

#elif defined(EPOLL) // Linux only
// https://man7.org/linux/man-pages/man7/epoll.7.html
#include <sys/epoll.h>
#endif


#define LEN_ARRAY(x) (sizeof(x)/sizeof(x[0]))
#define LEN_STR(x) (sizeof(x)/sizeof(x[0])-1)

#define PORT_SERVER 2000
#define IP_SERVER   "127.0.0.1"
#define SIZE_BUFFER 1024
#define CNT_TRYOUTS 20
#define TIMEOUT     5 // seconds




#ifdef SELECT
void handle_connections(int server_fd) {
    uint8_t buffer[SIZE_BUFFER] = { 0 };
    struct timeval tv = { .tv_sec = TIMEOUT, .tv_usec = 0, }; // Wait five seconds.
    fd_set rfds;
    fd_set set;
    FD_ZERO(&rfds);
    FD_ZERO(&set);
    FD_SET(server_fd, &set);
    int cnt_fds = 1;
    int cnt = 0;
    while (cnt < CNT_TRYOUTS) {
        rfds = set;
        int rc = select(FD_SETSIZE, &rfds, NULL, NULL, &tv);
        if (rc < 0) {
            printf("Failed to select: %s\n", strerror(errno));
            continue;
        } else if (rc == 0) {
            continue; // timeout
        }
        cnt++;
        for (int fd = 0; fd < FD_SETSIZE; fd++) {
            if (FD_ISSET(fd, &rfds) == 0) {
                continue;
            }
            if (fd == server_fd) {
                struct sockaddr_in client_addr = { 0 };
                unsigned int address_size = sizeof(client_addr);
                const int connect_fd = accept(server_fd, (struct sockaddr*) &client_addr, &address_size);
                if (connect_fd < 0) {
                    printf("Failed to receive connection\n");
                    continue;
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
                if (cnt_fds + 1 == FD_SETSIZE) {
                    printf("Can't add socket descriptor to set: set is full\n");
                    close(connect_fd);
                    continue;
                }
                FD_SET(connect_fd, &set);
                cnt_fds++;
            } else {
                const int received = recv(fd, buffer, LEN_ARRAY(buffer), MSG_DONTWAIT);
                if (received <= 0) {
                    FD_CLR(fd, &set);
                    close(fd);
                    cnt_fds--;
                    continue;
                }
                printf("Received %d bytes: %s\n", received, buffer);

                const size_t to_send = strlen((char*) buffer);
                const int sent = send(fd, buffer, to_send, 0);
                printf("Sent %d bytes from %zu\n", sent, to_send);
                if (sent < 0 || (unsigned int) sent < to_send) {
                    printf("Failed to send message to client: %s\n", strerror(errno));
                }
                memset(buffer, 0, sizeof(buffer));
            }
        }
    }
}
#elif defined(POLL)
#define MAX_FDS 1024

void handle_connections(int server_fd) {
    uint8_t buffer[SIZE_BUFFER] = { 0 };
    nfds_t nfds = 0;
    struct pollfd pollfds[MAX_FDS] = { 0 };

    pollfds->fd = server_fd;
    pollfds->events = POLLIN;
    pollfds->revents = 0;
    nfds_t maxfds = MAX_FDS;
    nfds_t numfds = 1;

    int cnt = 0;
    while (cnt < CNT_TRYOUTS) {
        nfds = numfds;
        int rc = poll(pollfds, nfds, TIMEOUT);
        if (rc < 0) {
            printf("Failed to poll: %s\n", strerror(errno));
            continue;
        } else if (rc == 0) {
            continue; // timeout
        }
        cnt++;
        for (unsigned int i = 0; i <= nfds; i++) {
            if (pollfds[i].fd <= 0) {
                continue;
            }
            // fd is ready for reading
            if ((pollfds[i].revents & POLLIN) == POLLIN) {
                if (pollfds[i].fd == server_fd) {
                    struct sockaddr_in client_addr = { 0 };
                    unsigned int address_size = sizeof(client_addr);
                    const int connect_fd = accept(server_fd, (struct sockaddr*) &client_addr, &address_size);
                    if (connect_fd < 0) {
                        printf("Failed to receive connection\n");
                        continue;
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
                    if (nfds == maxfds) {
                        printf("Can't add socket descriptor to set: set is full\n");
                        close(connect_fd);
                        continue;
                    }
                    pollfds[numfds].fd = connect_fd;
                    pollfds[numfds].events = POLLIN;
                    pollfds[numfds].revents = 0;
                    numfds++;
                } else {
                    const int received = recv(pollfds[i].fd, buffer, LEN_ARRAY(buffer), MSG_DONTWAIT);
                    if (received <= 0) {
                        close(pollfds[i].fd);
                        pollfds[i].fd *= -1;
                        // pollfds[i].events = POLLIN;
                        // pollfds[i].revents = 0;
                        continue;
                    }
                    printf("Received %d bytes: %s\n", received, buffer);

                    const size_t to_send = strlen((char*) buffer);
                    const int sent = send(pollfds[i].fd, buffer, to_send, 0);
                    printf("Sent %d bytes from %zu\n", sent, to_send);
                    if (sent < 0 || (unsigned int) sent < to_send) {
                        printf("Failed to send message to client: %s\n", strerror(errno));
                    }
                    memset(buffer, 0, sizeof(buffer));
                }
            } else {
                printf("Event on socket %d: 0x%X\n", pollfds[i].fd, pollfds[i].revents);
            }
        }
    }
}
#elif defined(EPOLL)
#define MAX_FDS 1024

void handle_connections(int server_fd) {
    uint8_t buffer[SIZE_BUFFER] = { 0 };
    int efd = epoll_create1(0);
    if (efd < 0) {
        printf("Failed to create epoll set: %s\n", strerror(errno));
        return;
    }
    struct epoll_event ev, ep_event[MAX_FDS] = { 0 };
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, server_fd, &ev) < 0) {
        printf("Failed to register descriptor in epoll set: %s\n", strerror(errno));
        close(efd);
        return;
    }
    int cnt_fds = 1;
    int cnt = 0;
    while (cnt < CNT_TRYOUTS) {
        int rc = epoll_wait(efd, ep_event, MAX_FDS, TIMEOUT);
        if (rc < 0) {
            printf("Failed to wait on epoll set: %s\n", strerror(errno));
            continue;
        } else if (rc == 0) {
            continue; // timeout
        }
        cnt++;
        for (int i = 0; i < rc; i++) {
            // descriptor is ready for reading
            if ((ep_event[i].events & EPOLLIN) == EPOLLIN) {
                if (ep_event[i].data.fd == server_fd) {
                    struct sockaddr_in client_addr = { 0 };
                    unsigned int address_size = sizeof(client_addr);
                    const int connect_fd = accept(server_fd, (struct sockaddr*) &client_addr, &address_size);
                    if (connect_fd < 0) {
                        printf("Failed to receive connection\n");
                        continue;
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
                    if (cnt_fds + 1 == MAX_FDS) {
                        printf("Can't add socket descriptor to set: set is full\n");
                        close(connect_fd);
                        continue;
                    }
                    ev.events = EPOLLIN;
                    ev.data.fd = connect_fd;
                    if (epoll_ctl(efd, EPOLL_CTL_ADD, connect_fd, &ev) < 0) {
                        printf("Failed to register descriptor in epoll set: %s\n", strerror(errno));
                        close(connect_fd);
                        continue;
                    }
                    cnt_fds++;
                } else {
                    const int received = recv(ep_event[i].data.fd, buffer, LEN_ARRAY(buffer), MSG_DONTWAIT);
                    if (received <= 0) {
                        if (epoll_ctl(efd, EPOLL_CTL_DEL, ep_event[i].data.fd, NULL) < 0) {
                        printf("Failed to remove descriptor from epoll set: %s\n", strerror(errno));
                        }
                        close(ep_event[i].data.fd);
                        continue;
                    }
                    printf("Received %d bytes: %s\n", received, buffer);

                    const size_t to_send = strlen((char*) buffer);
                    const int sent = send(ep_event[i].data.fd, buffer, to_send, 0);
                    printf("Sent %d bytes from %zu\n", sent, to_send);
                    if (sent < 0 || (unsigned int) sent < to_send) {
                        printf("Failed to send message to client: %s\n", strerror(errno));
                    }
                    memset(buffer, 0, sizeof(buffer));
                }
            } else {
                printf("Event on socket %d: 0x%X\n", ep_event[i].data.fd, ep_event[i].events);
            }
        }
    }
    close(efd);
}
#endif


int main(void) {
    const int reuse = 1;
    struct sockaddr_in name = { 0 };
    int server_fd = -1;
    int rc = -1;

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
    handle_connections(server_fd);

    close(server_fd);
    server_fd = -1;

    return EXIT_SUCCESS;
}
