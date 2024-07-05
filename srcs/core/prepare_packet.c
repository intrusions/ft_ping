#include "inc.h"

void prepare_packet(t_data *data, t_packet *packet, u16 n_sequence)
{
    memset(packet, 0, sizeof(*packet));
    packet->hdr.type = ICMP_ECHO;
    packet->hdr.code = 0;
    packet->hdr.un.echo.id = data->pid;
    packet->hdr.un.echo.sequence = n_sequence;

    for (u32 j = 0; j < sizeof(packet->msg) - 1; j++) {
        packet->msg[j] = j + '0';
    }
    packet->msg[sizeof(packet->msg) - 1] = 0;

    packet->hdr.checksum = checksum(packet, sizeof(*packet));
    // print_sended_packet(packet);
}