#include "inc.h"

static i8 verify_packet_integrity(t_data *data, struct iphdr *ip_hdr, struct icmphdr *icmp_hdr, u16 n_sequence, u8 *ttl)
{
    if (icmp_hdr->type == ICMP_ECHOREPLY &&
        icmp_hdr->un.echo.id == data->pid &&
        icmp_hdr->un.echo.sequence == n_sequence - 1) {
        *ttl = ip_hdr->ttl;
        return ICMP_PACKET_SUCCESS;
    }

    if (icmp_hdr->type == ICMP_TIME_EXCEEDED && icmp_hdr->code == ICMP_EXC_TTL) {
        return ERR_ICMP_TIME_EXCEEDED;
    }

    if (icmp_hdr->type == ICMP_DEST_UNREACH) {
        switch (icmp_hdr->code) {
            case ICMP_NET_UNREACH:  return ERR_ICMP_NET_UNREACH;
            case ICMP_HOST_UNREACH: return ERR_ICMP_HOST_UNREACH;
            case ICMP_PROT_UNREACH: return ERR_ICMP_PROT_UNREACH;
            case ICMP_PORT_UNREACH: return ERR_ICMP_PORT_UNREACH;
            default:                return ERR_ICMP_DEFAULT;
        }
    }

    return ERR_ICMP_DEFAULT;
}

void print_recv_err(i8 status_code, u8 packet_size, char *ip, u8 icmphdr_code)
{
    switch (status_code) {

        case ERR_ICMP_TIME_EXCEEDED:
            fprintf(stdout, "%d bytes from %s: Time to Live exceeded (TTL expired)\n", packet_size, ip);
            break;
        
        case ERR_ICMP_NET_UNREACH:
            fprintf(stdout, "%d bytes from %s: Destination Net Unreachable\n", packet_size, ip);
            break;
        
        case ERR_ICMP_HOST_UNREACH:
            fprintf(stdout, "%d bytes from %s: Destination Host Unreachable\n", packet_size, ip);
            break;
        
        case ERR_ICMP_PROT_UNREACH:
            fprintf(stdout, "%d bytes from %s: Destination Protocol Unreachable\n", packet_size, ip);
            break;
        
        case ERR_ICMP_PORT_UNREACH:
            fprintf(stdout, "%d bytes from %s: Destination Port Unreachable\n", packet_size, ip);
            break;
        
        default:
            fprintf(stdout, "%d bytes from %s: Destination Unreachable, code=%d\n", packet_size, ip, icmphdr_code);
            break;
    }
}

void recv_packet(t_data *data, struct sockaddr_in *r_addr, socklen_t *addr_len, char *ip, u16 n_sequence, u16 *n_packet_received, struct timeval *start_time, struct timeval *end_time)
{
    char response[PING_MAX_PKT_SIZE];
    u8 ttl;
    i32 bytes_received = 0;
    memset(response, 0, sizeof(response));
    
    if ((bytes_received = recvfrom(data->sockfd, response, sizeof(response), 0, (struct sockaddr *)r_addr, addr_len)) <= 0) {
        fprintf(stderr, "packet receive failed\n");
        close(data->sockfd);
        exit(EXIT_FAILURE);
    }
    
    struct iphdr *ip_hdr = (struct iphdr *)response;
    struct icmphdr *icmp_hdr = (struct icmphdr *)(response + (ip_hdr->ihl * 4));
    u8 packet_size = bytes_received - sizeof(struct iphdr);
    // print_received_packet(ip_hdr, icmp_hdr, response + 28);

    i8 status_code = verify_packet_integrity(data, ip_hdr, icmp_hdr, n_sequence, &ttl);
    if (status_code == ICMP_PACKET_SUCCESS) {
        ++*n_packet_received;
        gettimeofday(end_time, NULL);
        double time_elapsed = calcul_latency(*start_time, *end_time);
        fprintf(stdout, "%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n", packet_size, ip, n_sequence, ttl, time_elapsed);
    } else {
        print_recv_err(status_code, packet_size, ip, icmp_hdr->code);
    }
}