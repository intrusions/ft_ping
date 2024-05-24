#include "inc.h"

void print_recv_packet(t_packet *packet)
{
    fprintf(stdout, "packet->hdr.type:             %d\n", packet->hdr.type);
    fprintf(stdout, "packet->hdr.un.echo.id:       %d\n", packet->hdr.un.echo.id);
    fprintf(stdout, "packet->hdr.un.echo.sequence: %d\n", packet->hdr.un.echo.sequence);
}