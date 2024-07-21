#include "inc.h"

bool    reverse_dns(char *addr_in, char *addr)
{
    addrinfo hints;
    addrinfo *addr_info, *ptr;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    i32 status;
    if ((status = getaddrinfo(addr_in, NULL, &hints, &addr_info)) != 0) {
        __log_error("getaddrinfo error");
        return false;
    }

    for (ptr = addr_info; ptr; ptr = ptr->ai_next) {
        void *curr_addr;

        if (ptr->ai_family == AF_INET) {
            sockaddr_in *ipv4 = (sockaddr_in *)ptr->ai_addr;
            curr_addr = &(ipv4->sin_addr);

            if (!inet_ntop(ptr->ai_family, curr_addr, addr, INET6_ADDRSTRLEN)) {
                __log_error("inet_ntop error");
                return false;
            }
            break ;
        }
    }

    freeaddrinfo(addr_info);
    return true;
}
