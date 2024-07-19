#include "inc.h"

void close_sockfd_and_exit(t_data *data)
{
    close(data->sockfd);
    exit(EXIT_FAILURE);
}