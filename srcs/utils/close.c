#include "inc.h"

void close_sockfd_and_exit(t_data *data)
{
    if (data->sockfd > 0)
        close(data->sockfd);
    exit(EXIT_FAILURE);
}
