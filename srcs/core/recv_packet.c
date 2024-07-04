#include "inc.h"

static bool verify_packet_integrity(t_data *data, t_packet *packet)
{
    print_recv_packet(packet);
    struct iphdr *ip_hdr = (struct iphdr *)packet;
    struct icmphdr *icmp_hdr = (struct icmphdr *)(packet + (ip_hdr->ihl * 4));

    /* need other checks and dedicated messages on each one of them */
    if (icmp_hdr->type == ICMP_ECHOREPLY && icmp_hdr->un.echo.id == data->pid) {
        return true;
    }
    return false;
}

void recv_packet(t_data *data, t_packet *packet, struct sockaddr_in *r_addr, socklen_t *addr_len, char *ip, u16 n_sequence, u16 *n_packet_received, struct timeval *start_time, struct timeval *end_time)
{
    i32 bytes_received = 0;
    if ((bytes_received = recvfrom(data->sockfd, packet, sizeof(*packet), 0, (struct sockaddr *)r_addr, addr_len)) <= 0) {
        fprintf(stderr, "packet receive failed\n");
        close(data->sockfd);
        exit(EXIT_FAILURE);
    }
    /* 28 bytes received problem (ip header(20 bytes) + icmp header(8 bytes)), where is payload??? */
    /* icmp header is also empty */

    if (verify_packet_integrity(data, packet)) {
        ++*n_packet_received;
        gettimeofday(end_time, NULL);

        double time_elapsed = calcul_latency(*start_time, *end_time);
        fprintf(stdout, "%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n", PING_PKT_SIZE, ip, n_sequence, 42, time_elapsed);
    }
}