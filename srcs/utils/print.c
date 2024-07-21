#include "inc.h"

void print_man(void)
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
    fprintf(stdout, "PING %s (%s): 56 data bytes", data->addr_in, data->addr);

    if (data->flags & FLAG_V)
        fprintf(stdout, ", id 0x%04x = %d", data->pid, data->pid);
    fprintf(stdout, "\n");
}

void print_statistics(u16 n_sequence, u16 n_packet_received, char *addr_in, t_times *times)
{
    i32 packet_loss = (i32)(((n_sequence - n_packet_received) / (float)n_sequence) * 100);
    
    fprintf(stdout, "--- %s ping statistics ---\n", addr_in);
    fprintf(stdout, "%d packets transmitted, %d packets received, %d%% packet loss\n",
                n_sequence, n_packet_received, packet_loss);

    if (times->arr) {
        double avg, stddev, min, max;
        
        calcul_statistics(times, &min, &max, &avg, &stddev);
        fprintf(stdout, "round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
                    min, avg, max, stddev);
    }
}

// IP Hdr Dump:
//  0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 
// Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src	Dst	Data
//  0  0  00 0000 0000   0 0000  00  00 0000 0.0.0.0  0.0.0.0 
// ICMP: type 0, code 0, size 1, id 0x0800, seq 0x0000
void print_verbose_option(iphdr *ip_hdr, icmphdr *icmp_hdr)
{
    u8 *cp = (u8 *) ip_hdr + sizeof(iphdr);
    u8 *ip_header = (u8 *)ip_hdr;
    u64 hlen = ip_hdr->ihl * 4;

    printf("IP Hdr Dump:\n");
    for (u32 i = 0; i < sizeof(iphdr); i++) {

        printf("%02x", ip_header[i]);
        if (i % 2)
            printf(" ");
    }
    printf("\n");

    printf("Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src\tDst\tData\n");
    printf(" %1x  %1x  %02x %04x %04x   %1x %04x  %02x  %02x %04x %s  %s ",
                ip_hdr->version,                                //Vr
                ip_hdr->ihl,                                    //HL
                ip_hdr->tos,                                    //TOS
                ntohs(ip_hdr->tot_len),                         //Len
                ntohs(ip_hdr->id),                              //ID
                (ntohs(ip_hdr->frag_off) & 0xe000) >> 13,       //Flg
                ntohs(ip_hdr->frag_off) & 0x1fff,               //off
                ip_hdr->ttl,                                    //TTL
                ip_hdr->protocol,                               //Pro
                ntohs(ip_hdr->check),                           //cks
                __ip_str(ip_hdr->saddr),                        //Src
                __ip_str(ip_hdr->daddr));                       //Dst


    while (hlen-- > sizeof(iphdr))
        printf("%02x", *cp++);                                  //Data
    printf("\n");

    printf("ICMP: type %u, code %u, size %lu, id 0x%04x, seq 0x%04x\n",
                icmp_hdr->type,                                             //Type
                icmp_hdr->code,                                             //Code
                ntohs(ip_hdr->tot_len) - (hlen + sizeof(iphdr)),            //Size
                ntohs(icmp_hdr->un.echo.id),                                //ID
                ntohs(icmp_hdr->un.echo.sequence));                         //Sequence
}
