#include "inc.h"

bool    reverse_dns(char *hostname_in, char *hostname)
{
    struct addrinfo hints;
    struct addrinfo *addr_info, *ptr;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    i32 status;
    if ((status = getaddrinfo(hostname_in, NULL, &hints, &addr_info)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return true;
    }

    for (ptr = addr_info; ptr; ptr = ptr->ai_next) {
        void *addr;

        if (ptr->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)ptr->ai_addr;
            addr = &(ipv4->sin_addr);
            inet_ntop(ptr->ai_family, addr, hostname, INET6_ADDRSTRLEN);
            break ;
        }
    }

    freeaddrinfo(addr_info);
    return false;
}