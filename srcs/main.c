#include "inc.h"

bool SIG_EXIT;

static void sig_handler(const int signal)
{
    if (signal == SIGINT) {
        SIG_EXIT = true;
    }
}

static bool manage_flags(i32 ac, char **av, u8 *flags)
{
    for (i32 index = 0; index < ac; index++) {
        
        if (!strcmp(av[index], "-?") || !strcmp(av[index], "-h") || !strcmp(av[index], "--help")) {
            print_man();
            return (1);
        } else if (!strcmp(av[index], "-v") || !strcmp(av[index], "--verbose")) {
            *flags |= FLAG_V;
        }
    }
    return (0);
}

static void init_dest(t_data *data, char *hostname)
{
    (void)data;
    printf("%s\n", hostname);
}

int main(int ac, char **av)
{
    u8      flags = 0x0;
    t_data data;

    memset(&data, 0, sizeof(data));

    if (ac == 1) {
        fprintf(stderr, "ping: usage error: Destination address required\n");
        return (EXIT_FAILURE);
    }

    av++, ac--;

    if (manage_flags(ac, av, &flags)) {
        return (EXIT_FAILURE);
    }

    signal(SIGINT, sig_handler);

    data.pid = getpid();
    data.minMS = 1;

    for (i32 index = 0; index < ac; index++) {

        if (av[index][0] != '-') {
            init_dest(&data, av[index]);
        }
    }
}
