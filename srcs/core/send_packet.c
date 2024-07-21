#include "inc.h"

void send_packet(t_data *data, t_packet *packet, timeval *start_time, u16 *n_sequence)
{
    gettimeofday(start_time, NULL);
    if (sendto(data->sockfd, packet, sizeof(*packet), 0, (sockaddr *)&data->dest, sizeof(data->dest)) <= 0) {
        __log_error("sendto error");
        close_sockfd_and_exit(data);
    }
    ++*n_sequence;

    if (data->flags & FLAG_D)
        print_sent_packet(packet);
}
