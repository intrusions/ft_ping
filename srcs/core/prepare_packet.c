#include "inc.h"

void prepare_packet(t_data *data, t_packet *packet, u16 n_sequence)
{
    memset(packet, 0, sizeof(*packet));
    packet->hdr.type = ICMP_ECHO;
    packet->hdr.code = 0;
    packet->hdr.un.echo.id = data->pid;
    packet->hdr.un.echo.sequence = n_sequence;

    u32 payload_size = sizeof(packet->payload);
    u32 pattern_len = strlen(data->option.option_pattern_value);

    if (data->flags & FLAG_P) {
        for (u32 i = 0; i < payload_size; ) {
            for (u32 j = 0; j < pattern_len && i < payload_size; j++, i++) {
                packet->payload[i] = data->option.option_pattern_value[j];
            }
        }
    } else {
        for (u32 j = 0; j < sizeof(packet->payload) - 1; j++) {
            packet->payload[j] = j + '0';
        }
    }

    packet->hdr.checksum = checksum(packet, sizeof(*packet));
}
