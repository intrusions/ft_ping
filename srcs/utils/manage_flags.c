#include "inc.h"

bool manage_flags(i32 ac, char **av, u8 *flags)
{
    for (i32 index = 0; index < ac; index++) {
        
        if (!strcmp(av[index], "-?") || !strcmp(av[index], "-h") || !strcmp(av[index], "--help")) {
            print_man();
            return false;
        } else if (!strcmp(av[index], "-v") || !strcmp(av[index], "--verbose")) {
            *flags |= FLAG_V;
        } else if (!strcmp(av[index], "-d") || !strcmp(av[index], "--debug")) {
            *flags |= FLAG_D;
        } else if (!strcmp(av[index], "-q")) {
            *flags |= FLAG_Q;
        }
    }
    return true;
}
