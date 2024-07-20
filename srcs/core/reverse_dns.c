#include "inc.h"

bool    reverse_dns(char *hostname_in, char *hostname)
{
    addrinfo hints;
    addrinfo *addr_info, *ptr;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    i32 status;
    if ((status = getaddrinfo(hostname_in, NULL, &hints, &addr_info)) != 0) {
        __log_error("getaddrinfo error");
        return false;
    }

    for (ptr = addr_info; ptr; ptr = ptr->ai_next) {
        void *addr;

        if (ptr->ai_family == AF_INET) {
            sockaddr_in *ipv4 = (sockaddr_in *)ptr->ai_addr;
            addr = &(ipv4->sin_addr);

            if (!inet_ntop(ptr->ai_family, addr, hostname, INET6_ADDRSTRLEN)) {
                __log_error("inet_ntop error");
                return false;
            }
            break ;
        }
    }

    freeaddrinfo(addr_info);
    return true;
}
