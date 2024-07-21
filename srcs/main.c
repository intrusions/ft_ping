#include "inc.h"

bool SIG_EXIT;

static void sig_handler(const int signal)
{
    if (signal == SIGINT) {
        SIG_EXIT = true;
    }
}

static void send_ping(t_data *data)
{
    sockaddr_in r_addr;
    t_times times;
    u16 n_sequence = 0;
    u16 n_packet_received = 0;
    
    memset(&times, 0, sizeof(t_times));

    print_header(data);
    while (!SIG_EXIT) {
        t_packet packet;
        timeval start_time, end_time;
        
        prepare_packet(data, &packet, n_sequence);
        send_packet(data, &packet, &start_time, &n_sequence);
        recv_packet(data, &r_addr, n_sequence, &n_packet_received, &start_time, &end_time, &times);

        usleep(PING_SENDING_DELAY * 1000000);
    }
    
    print_statistics(n_sequence, n_packet_received, data->addr_in, &times);
    free_times(&times);
}

int main(int ac, char **av)
{
    t_data data;
    memset(&data, 0, sizeof(data));
    data.addr_in = av[ac - 1];

    av++, ac--;

    if (!ac) {
        fprintf(stderr, "ping: usage error: Destination address required\n");
        return EXIT_FAILURE;
    }

    if (!manage_flags(ac, av, &data.flags))
        return EXIT_SUCCESS;
    
    if (!reverse_dns(data.addr_in, data.addr))
        return EXIT_FAILURE;
    
    if (!initialization(&data))
        close_sockfd_and_exit(&data);

    signal(SIGINT, sig_handler);
    send_ping(&data);
    close(data.sockfd);

    return EXIT_SUCCESS;
}
