#include "inc.h"

void print_man()
{
    printf(
        "\n"
        "Usage\n"
        "   ping [options] <destination>\n"
        "\n"
        "Options:\n"
        "   <destination>      dns name or ip address\n"
        "   -v                 verbose output\n"
        "   -? -h --help       print manual\n"
    );
}

void print_header(t_data *data)
{
    fprintf(stdout, "PING %s (%s): 56 data bytes", data->hostname_in, data->hostname);

    if (data->flags & FLAG_V) {
        fprintf(stdout, ", id 0x%04x = %d", data->pid, data->pid);
    }
    fprintf(stdout, "\n");
}

void print_statistics(u16 n_sequence, u16 n_packet_received, struct timeval ping_start_time, struct timeval ping_end_time, char *hostname_in, t_times *times)
{
    double  time_elapsed = calcul_latency(ping_start_time, ping_end_time);
    i32     packet_loss = (int)(((n_sequence - n_packet_received) / (float)n_sequence) * 100);
    
    double avg, stddev, min, max;
    calcul_statistics(times, &min, &max, &avg, &stddev);

    fprintf(stdout, "--- %s ping statistics ---\n", hostname_in);
    fprintf(stdout, "%d packets transmitted, %d received, %d%% packet loss, time %.0fms\n", n_sequence, n_packet_received, packet_loss, time_elapsed);
    fprintf(stdout, "round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n", min, avg, max, stddev);
}

// IP Hdr Dump:
//  0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 
// Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src	Dst	Data
//  0  0  00 0000 0000   0 0000  00  00 0000 0.0.0.0  0.0.0.0 
// ICMP: type 0, code 0, size 1, id 0x0800, seq 0x0000
// void print_verbose_option(struct iphdr *ip_hdr, struct icmphdr *icmp_hdr)
// {
//     (void)ip_hdr;
//     (void)icmp_hdr;
//     return ;
// }

void print_verbose_option(struct iphdr *ip_hdr, struct icmphdr *icmp_hdr)
{
    char src_ip[INET_ADDRSTRLEN], dst_ip[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &ip_hdr->saddr, src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &ip_hdr->daddr, dst_ip, INET_ADDRSTRLEN);

    printf("IP Hdr Dump:\n");
    printf("  Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src       Dst\n");
    printf("  %1d  %1d  %02x %04x %04x   %1x %04x  %02x  %02x %04x %s %s\n",
           ip_hdr->version,
           ip_hdr->ihl,
           ip_hdr->tos,
           ntohs(ip_hdr->tot_len),
           ntohs(ip_hdr->id),
           ip_hdr->frag_off >> 13,
           ntohs(ip_hdr->frag_off & 0x1FFF),
           ip_hdr->ttl,
           ip_hdr->protocol,
           ntohs(ip_hdr->check),
           src_ip,
           dst_ip);

    printf("ICMP: type %d, code %d, checksum 0x%04x, id 0x%04x, seq 0x%04x\n",
           icmp_hdr->type,
           icmp_hdr->code,
           ntohs(icmp_hdr->checksum),
           ntohs(icmp_hdr->un.echo.id),
           ntohs(icmp_hdr->un.echo.sequence));
}
