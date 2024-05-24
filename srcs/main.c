#include "inc.h"

bool SIG_EXIT;

static void sig_handler(const int signal)
{
    if (signal == SIGINT) {
        SIG_EXIT = true;
    }
}

static void send_ping(t_data *data, char *ip)
{
    struct sockaddr_in r_addr;
    socklen_t addr_len = sizeof(r_addr);
    u16 n_sequence = 0;
    struct timeval start_time, end_time;

    while (!SIG_EXIT) {
        t_packet packet;
        ++n_sequence;
        
        prepare_packet(data, &packet, n_sequence);
        send_packet(data, &packet, &start_time);
        recv_packet(data->sockfd, &packet, &r_addr, &addr_len, ip, n_sequence, &start_time, &end_time);

        usleep(data->delay * 1000000);
    }
}

static bool initialization(int ac, char **av, t_data *data)
{
    i32 optval = 1;
    data->pid = getpid();
    data->delay = 1;
    memset(&data->dest, 0, sizeof(data->dest));
    data->dest.sin_family = AF_INET;
    
    if (inet_pton(AF_INET, av[ac - 1], &data->dest.sin_addr) <= 0) {
        fprintf(stderr, "invalid address\n");
        return true;
    }

    if (setsockopt(data->sockfd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) != 0) {
        fprintf(stderr, "error setting socket options\n");
        return true;
    }
    return false;
}

int main(int ac, char **av)
{
    u8      flags = 0x0;
    t_data data;

    memset(&data, 0, sizeof(data));

    av++, ac--;

    if (!ac) {
        fprintf(stderr, "ping: usage error: Destination address required\n");
        return EXIT_FAILURE;
    }
    if (manage_flags(ac, av, &flags)) {
        return EXIT_SUCCESS;
    }
    if ((data.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
        fprintf(stderr, "socket creation failed\n");
        return EXIT_FAILURE;
    }
    if (initialization(ac, av, &data)) {
        return EXIT_FAILURE;
    }

    signal(SIGINT, sig_handler);
    send_ping(&data, av[ac - 1]);
    close(data.sockfd);
    return EXIT_SUCCESS;
}
