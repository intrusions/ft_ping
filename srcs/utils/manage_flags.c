#include "inc.h"

static bool set_option_value(t_data *data, i32 index, i32 ac, char **av, u8 flag)
{
    if (!(index + 1 < ac)) {
        fprintf(stderr, "ping: option requires an argument -- '%s'\n", av[index]);
        return false;
    }

    if (flag & FLAG_P) {

        if (strlen(av[index + 1]) < PING_MAX_PATTERN_SIZE) {
            strncpy(data->option.option_pattern_value, av[index + 1], strlen(av[index + 1]));
        } else {
            strncpy(data->option.option_pattern_value, av[index + 1], 16);
        }
        data->flags |= flag;
        return true;
    } else {

        u8 len = strlen(av[++index]);
        for (u8 i = 0; i < len; i++) {
            if (!isdigit(av[index][i])) {
                fprintf(stderr, "ping: invalid value (`%s')\n", av[index]);
                return false;
            }
        }

        data->flags |= flag;

        if (!strcmp(av[index - 1], "-i")) {
            data->option.option_delay_value = atoi(av[index]);
        } else if (!strcmp(av[index - 1], "-c")) {
            data->option.option_count_value = atoi(av[index]);
        }
    }
    
    return true;
}

bool manage_flags(t_data *data, i32 ac, char **av)
{
    for (i32 index = 0; index < ac; index++) {
        
        if (!strcmp(av[index], "-?") || !strcmp(av[index], "-h") || !strcmp(av[index], "--help")) {
            print_man();
            exit(EXIT_SUCCESS);
        } else if (!strcmp(av[index], "-v") || !strcmp(av[index], "--verbose")) {
            data->flags |= FLAG_V;
        } else if (!strcmp(av[index], "-d") || !strcmp(av[index], "--debug")) {
            data->flags |= FLAG_D;
        } else if (!strcmp(av[index], "-q")) {
            data->flags |= FLAG_Q;
        } else if (!strcmp(av[index], "-i") && !set_option_value(data, index, ac, av, FLAG_I)) {
            return false;
        } else if (!strcmp(av[index], "-c") && !set_option_value(data, index, ac, av, FLAG_C)) {
            return false;
        } else if (!strcmp(av[index], "-p") && !set_option_value(data, index, ac, av, FLAG_P)) {
            return false;
        }
    }
    return true;
}
