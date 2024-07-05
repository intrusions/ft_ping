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

    if (data->flags & FLAG_V)
        fprintf(stdout, ", id 0x%04x = %d", data->pid, data->pid);
    fprintf(stdout, "\n");
}

void print_statistics(u16 n_sequence, u16 n_packet_received, struct timeval ping_start_time, struct timeval ping_end_time, char *hostname_in)
{
    double  time_elapsed = calcul_latency(ping_start_time, ping_end_time);
    i32     packet_loss = (int)(((n_sequence - n_packet_received) / (float)n_sequence) * 100);
    
    fprintf(stdout, "--- %s ping statistics ---\n", hostname_in);
    fprintf(stdout, "%d packets transmitted, %d received, %d%% packet loss, time %.0fms\n", n_sequence, n_packet_received, packet_loss, time_elapsed);
}