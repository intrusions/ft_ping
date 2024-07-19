#include "inc.h"

static void print_recv_err(i8 status_code, u8 packet_size, char *ip, u8 icmphdr_code)
{   
    switch (status_code) {

        case ERR_ICMP_TIME_EXCEEDED:
            fprintf(stdout, "%d bytes from %s: Time to Live exceeded\n", packet_size, ip);
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

static i8 verify_packet_integrity(t_data *data, iphdr *ip_hdr, icmphdr *icmp_hdr, u16 n_sequence, u8 *ttl)
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

void recv_packet(t_data *data, sockaddr_in *r_addr, socklen_t *addr_len, u16 n_sequence, u16 *n_packet_received, timeval *start_time, timeval *end_time, t_times *times)
{
    char response[PING_MAX_PKT_SIZE];
    u8 ttl;
    i32 bytes_received;
    u8 packet_size;

    iphdr *ip_hdr;
    icmphdr *icmp_hdr;

    memset(response, 0, sizeof(response));

    timeval tv = {0, 100000};

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(data->sockfd, &readfds);

    int retval = select(data->sockfd + 1, &readfds, NULL, NULL, &tv);
    if (retval == -1) {
        fprintf(stderr, "select failed\n");
        close(data->sockfd);
        exit(EXIT_FAILURE);
    } else if (retval == 0) {
        return ;
    }

    do {
        if ((bytes_received = recvfrom(data->sockfd, response, sizeof(response), 0, (sockaddr *)r_addr, addr_len)) <= 0) {
            fprintf(stderr, "packet receive failed\n");
            close(data->sockfd);
            exit(EXIT_FAILURE);
        }
        
        ip_hdr = (iphdr *)response;
        icmp_hdr = (icmphdr *)(response + (ip_hdr->ihl * 4));
        packet_size = bytes_received - sizeof(iphdr);

        if (data->flags & FLAG_D)
            print_received_packet(ip_hdr, icmp_hdr, response + sizeof(*ip_hdr) + sizeof(*icmp_hdr));
            
    } while (icmp_hdr->type == 8 && (!strcmp(IP_STR(ip_hdr->saddr), IP_STR(ip_hdr->daddr))));

    i8 status_code = verify_packet_integrity(data, ip_hdr, icmp_hdr, n_sequence, &ttl);
    if (status_code == ICMP_PACKET_SUCCESS) {
        ++*n_packet_received;
        gettimeofday(end_time, NULL);
        double time_elapsed = calcul_latency(*start_time, *end_time);
        
        fprintf(stdout, "%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
                        packet_size,
                        IP_STR(ip_hdr->saddr),
                        n_sequence,
                        ttl,
                        time_elapsed);
        
        push_time(times, time_elapsed);
    } else {
        print_recv_err(status_code, packet_size, IP_STR(ip_hdr->saddr), icmp_hdr->code);
        
        if (data->flags & FLAG_V)
            print_verbose_option(ip_hdr, icmp_hdr);
    }
}
