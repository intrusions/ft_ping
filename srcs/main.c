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
    for (i32 index = 1; index < ac; index++) {
        
        if (!strcmp(av[index], "-?") || !strcmp(av[index], "-h") || !strcmp(av[index], "--help")) {
            print_man();
            return (1);
        }
    
        if (!strcmp(av[index], "-v") || !strcmp(av[index], "--verbose")) {
            *flags |= FLAG_V;
        }
    }
    return (0);
}

int main(int ac, char **av)
{
    u8 flags = 0x0;

    if (ac == 1) {
        fprintf(stderr, "ping: usage error: Destination address required\n");
        return (EXIT_FAILURE);
    }

    if (manage_flags(ac, av, &flags)) {
        return (EXIT_FAILURE);
    }

    signal(SIGINT, sig_handler);
}
