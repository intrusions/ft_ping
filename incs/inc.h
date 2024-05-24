#ifndef INC_H
# define INC_H

// ========================================================================= //
//                                   Header                                  //
// ========================================================================= //

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <fcntl.h>

// ========================================================================= //
//                                   Define                                  //
// ========================================================================= //

#define FLAG_V 0x01

#define PING_PKT_SIZE 64


// ========================================================================= //
//                                  Typedef                                  //
// ========================================================================= //

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;


// ========================================================================= //
//                                  Structure                                //
// ========================================================================= //

typedef struct {
    i32         sockfd;
    u16         pid;
    u32         delay;

    struct sockaddr_in dest;
} t_data;

typedef struct {
    struct icmphdr  hdr;
    char            msg[PING_PKT_SIZE - sizeof(struct icmphdr)];
} t_packet;


// ========================================================================= //
//                                  Prototype                                //
// ========================================================================= //

/* core */
void prepare_packet(t_data *data, t_packet *packet, u16 n_sequence);
void send_packet(t_data *data, t_packet *packet, struct timeval *start_time);
void recv_packet(u32 sockfd, t_packet *packet, struct sockaddr_in *r_addr, socklen_t *addr_len, char *ip, u16 n_sequence, struct timeval *start_time, struct timeval *end_time);


/* utils */
void    print_man();
bool    manage_flags(i32 ac, char **av, u8 *flags);
u16     checksum(void *b, int len);


/* debug */
void print_recv_packet(t_packet *packet);

#endif /* INC_H */