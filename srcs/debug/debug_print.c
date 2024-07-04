#include "inc.h"

void print_recv_packet(t_packet *packet)
{
    struct iphdr *ip_hdr = (struct iphdr *)packet;
    struct icmphdr *icmp_hdr = (struct icmphdr *)(packet + (ip_hdr->ihl * 4));
    
    printf("[ICMP Packet Debug Info]:\n");
    printf("[Packet received]:\n");
    printf("IP Header\n");
    printf(" |-IP Version        : %d\n", (unsigned int)ip_hdr->version);
    printf(" |-IP Header Length  : %d DWORDS or %d Bytes\n", (unsigned int)ip_hdr->ihl, ((unsigned int)(ip_hdr->ihl)) * 4);
    printf(" |-Type Of Service   : %d\n", (unsigned int)ip_hdr->tos);
    printf(" |-IP Total Length   : %d Bytes(Size of Packet)\n", ntohs(ip_hdr->tot_len));
    printf(" |-Identification    : %d\n", ntohs(ip_hdr->id));
    printf(" |-TTL      : %d\n", (unsigned int)ip_hdr->ttl);
    printf(" |-Protocol : %d\n", (unsigned int)ip_hdr->protocol);
    printf(" |-Checksum : %d\n", ntohs(ip_hdr->check));
    printf(" |-Source IP        : %s\n", inet_ntoa(*(struct in_addr *)&ip_hdr->saddr));
    printf(" |-Destination IP   : %s\n", inet_ntoa(*(struct in_addr *)&ip_hdr->daddr));

    printf("ICMP Header\n");
    printf(" |-Type : %d\n", (unsigned int)(icmp_hdr->type));
    printf(" |-Code : %d\n", (unsigned int)(icmp_hdr->code));
    printf(" |-Checksum : %d\n", ntohs(icmp_hdr->checksum));
    printf(" |-Identifier : %d\n", ntohs(icmp_hdr->un.echo.id));
    printf(" |-Sequence Number : %d\n", ntohs(icmp_hdr->un.echo.sequence));
    printf("Message\n");
    printf(" |-Content : %s\n", packet->msg);
    printf("--------------------------------------------\n\n");
}

void print_sended_packet(t_packet *packet)
{
    printf("[ICMP Packet Debug Info]:\n");
    printf("[Packet sended]:\n");
    printf(" |-Type        : %d\n", packet->hdr.type);
    printf(" |-Code        : %d\n", packet->hdr.code);
    printf(" |-Checksum    : %d\n", packet->hdr.checksum);
    printf(" |-Identifier  : %d\n", packet->hdr.un.echo.id);
    printf(" |-Sequence    : %d\n", packet->hdr.un.echo.sequence);

    printf("Message Data :\n");
    printf(" |-Size        : %lu bytes\n", sizeof(packet->msg));
    printf(" |-Content     : %s\n", packet->msg);
    printf("--------------------------------------------\n\n");
}