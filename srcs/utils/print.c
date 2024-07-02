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

void print_statistics(u16 n_sequence, u16 n_packet_received, struct timeval ping_start_time, struct timeval ping_end_time, char *ip)
{
    double time_elapsed = (ping_end_time.tv_sec - ping_start_time.tv_sec) * 1000.0;
    time_elapsed += (ping_end_time.tv_usec - ping_start_time.tv_usec) / 1000.0;

    i32 packet_loss = (int)(((n_sequence - n_packet_received) / (float)n_sequence) * 100);
    
    fprintf(stdout, "--- %s ping statistics ---\n", ip);
    fprintf(stdout, "%d packets transmitted, %d received, %d%% packet loss, time %.0fms\n", n_sequence, n_packet_received, packet_loss, time_elapsed);
}