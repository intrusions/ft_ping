#include "inc.h"

void send_packet(t_data *data, t_packet *packet, struct timeval *start_time, u16 *n_sequence)
{
    gettimeofday(start_time, NULL);
    if (sendto(data->sockfd, packet, sizeof(*packet), 0, (struct sockaddr *)&data->dest, sizeof(data->dest)) <= 0) {
        fprintf(stdout, "packet send failed\n");
        close(data->sockfd);
        exit(EXIT_FAILURE);
    }
    ++*n_sequence;
}