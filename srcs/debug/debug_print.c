#include "inc.h"

void print_sent_packet(t_packet *packet)
{
    printf("[Packet sent]\n");
    printf("[ICMP Header]\n");
    printf(" |-Type        : %d\n", packet->hdr.type);
    printf(" |-Code        : %d\n", packet->hdr.code);
    printf(" |-Checksum    : %d\n", packet->hdr.checksum);
    printf(" |-Identifier  : %d\n", packet->hdr.un.echo.id);
    printf(" |-Sequence    : %d\n", packet->hdr.un.echo.sequence);

    printf("[Message Data]\n");
    printf(" |-Size        : %lu bytes\n", sizeof(packet->msg));
    printf(" |-Content     : %s\n", packet->msg);
    printf("------------------------------------------------------------------------------\n\n");
}

void print_received_packet(struct iphdr *ip_header, struct icmphdr *icmp_header, char *msg) {
    printf("[Packet Received]\n");
    
    printf("[IP Header]\n");
    printf(" |-Version     : %d\n", ip_header->version);
    printf(" |-IHL         : %d (bytes)\n", ip_header->ihl * 4);
    printf(" |-TOS         : %d\n", ip_header->tos);
    printf(" |-Total Length: %d\n", ntohs(ip_header->tot_len));
    printf(" |-ID          : %d\n", ntohs(ip_header->id));
    printf(" |-Frag Offset : %d\n", ntohs(ip_header->frag_off) & 0x1FFF);
    printf(" |-TTL         : %d\n", ip_header->ttl);
    printf(" |-Protocol    : %d\n", ip_header->protocol);
    printf(" |-Checksum    : %d\n", ntohs(ip_header->check));
    printf(" |-Source IP   : %s\n", inet_ntoa(*(struct in_addr *)&ip_header->saddr));
    printf(" |-Dest IP     : %s\n", inet_ntoa(*(struct in_addr *)&ip_header->daddr));

    printf("[ICMP Header]\n");
    printf(" |-Type        : %d\n", icmp_header->type);
    printf(" |-Code        : %d\n", icmp_header->code);
    printf(" |-Checksum    : %d\n", icmp_header->checksum);
    printf(" |-Identifier  : %d\n", icmp_header->un.echo.id);
    printf(" |-Sequence    : %d\n", icmp_header->un.echo.sequence);

    printf("[Message Data]\n");
    printf(" |-Size        : %lu bytes\n", strlen(msg) + 1);
    printf(" |-Content     : %s\n", msg);
    printf("------------------------------------------------------------------------------\n\n");
}
