#include "inc.h"

void recv_packet(u32 sockfd, t_packet *packet, struct sockaddr_in *r_addr, socklen_t *addr_len, char *ip, u16 n_sequence, struct timeval *start_time, struct timeval *end_time)
{
    if (recvfrom(sockfd, packet, sizeof(*packet), 0, (struct sockaddr *) r_addr, addr_len) <= 0) {
        fprintf(stderr, "packet receive failed\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    } else {
        gettimeofday(end_time, NULL);

        double time_elapsed = (end_time->tv_sec - start_time->tv_sec) * 1000.0;
        time_elapsed += (end_time->tv_usec - start_time->tv_usec) / 1000.0;

        fprintf(stdout, "%d bytes from %s: icmp_seq=%d ttl=%d time=%.1f ms\n", PING_PKT_SIZE, ip, n_sequence, 42, time_elapsed);
    }
}