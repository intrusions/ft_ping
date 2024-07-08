#include "inc.h"

void prepare_packet(t_data *data, t_packet *packet, u16 n_sequence)
{
    memset(packet, 0, sizeof(*packet));
    packet->hdr.type = ICMP_ECHO;
    packet->hdr.code = 0;
    packet->hdr.un.echo.id = data->pid;
    packet->hdr.un.echo.sequence = n_sequence;

    for (u32 j = 0; j < sizeof(packet->payload) - 1; j++) {
        packet->payload[j] = j + '0';
    }
    packet->payload[sizeof(packet->payload) - 1] = 0;

    packet->hdr.checksum = checksum(packet, sizeof(*packet));
    
    if (data->flags & FLAG_D) {
        print_sent_packet(packet);
    }
}