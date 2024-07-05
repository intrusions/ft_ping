#include "inc.h"

static bool verify_packet_integrity(t_data *data, char *response, u16 n_sequence, u8 *ttl)
{
    struct iphdr *ip_hdr = (struct iphdr *)response;
    struct icmphdr *icmp_hdr = (struct icmphdr *)(response + (ip_hdr->ihl * 4));
    // print_received_packet(ip_hdr, icmp_hdr, response + 28);
    
    if (icmp_hdr->type == ICMP_ECHOREPLY &&
        icmp_hdr->un.echo.id == data->pid &&
        icmp_hdr->un.echo.sequence == n_sequence - 1) {
        *ttl = ip_hdr->ttl;
        return true;
    }
    /* need other checks and dedicated messages on each one of them */
    return false;
}

void recv_packet(t_data *data, struct sockaddr_in *r_addr, socklen_t *addr_len, char *ip, u16 n_sequence, u16 *n_packet_received, struct timeval *start_time, struct timeval *end_time)
{
    char buff[PING_MAX_PKT_SIZE];
    memset(buff, 0, sizeof(buff));
    
    i32 bytes_received = 0;
    if ((bytes_received = recvfrom(data->sockfd, buff, sizeof(buff), 0, (struct sockaddr *)r_addr, addr_len)) <= 0) {
        fprintf(stderr, "packet receive failed\n");
        close(data->sockfd);
        exit(EXIT_FAILURE);
    }

    u8 ttl;
    if (verify_packet_integrity(data, buff, n_sequence, &ttl)) {
        ++*n_packet_received;
        gettimeofday(end_time, NULL);

        double time_elapsed = calcul_latency(*start_time, *end_time);
        fprintf(stdout, "%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n", PING_PKT_SIZE, ip, n_sequence, ttl, time_elapsed);
    }
}