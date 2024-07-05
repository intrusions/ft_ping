#include "inc.h"

u16 checksum(void *b, int len)
{
    u16 *buf = b;
    u32 sum = 0;
    u16 result;

    for (sum = 0; len > 1; len -= 2) {
        sum += *buf++;
    }
    
    if (len == 1) {
        sum += *(u8*)buf;
    }
    
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    
    result = ~sum;
    return result;
}
