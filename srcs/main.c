#include "inc.h"

bool SIG_EXIT;

static void sig_handler(const int signal)
{
    if (signal == SIGINT) {
        SIG_EXIT = true;
        fprintf(stdout, "\n");
    }
}

static void send_ping(t_data *data, char *ip)
{
    struct sockaddr_in r_addr;
    socklen_t addr_len = sizeof(r_addr);
    u16 n_sequence = 0;
    u16 n_packet_received = 0;
    
    struct timeval ping_start_time;
    gettimeofday(&ping_start_time, NULL);

    t_times times;
    memset(&times, 0, sizeof(t_times));

    print_header(data);
    while (!SIG_EXIT) {
        t_packet packet;
        struct timeval start_time, end_time;
        
        prepare_packet(data, &packet, n_sequence);
        send_packet(data, &packet, &start_time, &n_sequence);
        recv_packet(data, &r_addr, &addr_len, data->hostname, n_sequence, &n_packet_received, &start_time, &end_time, &times);

        usleep(PING_SENDING_DELAY * 1000000);
    }
    struct timeval ping_end_time;
    gettimeofday(&ping_end_time, NULL);
    
    print_statistics(n_sequence, n_packet_received, ping_start_time, ping_end_time, ip, &times);
    free_times(&times);
}

static bool initialization(t_data *data)
{
    if ((data->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
        fprintf(stderr, "socket creation failed\n");
        return true;
    }

    data->pid = getpid();
    
    memset(&data->dest, 0, sizeof(data->dest));
    data->dest.sin_family = AF_INET;
    
    if (inet_pton(AF_INET, data->hostname, &data->dest.sin_addr) <= 0) {
        fprintf(stderr, "invalid address\n");
        return true;
    }

    i32 optval = 1;
    if (setsockopt(data->sockfd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) != 0) {
        fprintf(stderr, "error setting socket options\n");
        return true;
    }
    return false;
}

int main(int ac, char **av)
{
    t_data data;
    memset(&data, 0, sizeof(data));
    data.hostname_in = av[ac - 1];

    av++, ac--;

    if (!ac) {
        fprintf(stderr, "ping: usage error: Destination address required\n");
        return EXIT_FAILURE;
    }
    if (manage_flags(ac, av, &data.flags)) {
        return EXIT_SUCCESS;
    }
    if (reverse_dns(data.hostname_in, data.hostname)) {
        return EXIT_FAILURE;
    }
    if (initialization(&data)) {
        close(data.sockfd);
        return EXIT_FAILURE;
    }

    signal(SIGINT, sig_handler);
    send_ping(&data, av[ac - 1]);
    close(data.sockfd);

    return EXIT_SUCCESS;
}
