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
#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>


// ========================================================================= //
//                                   Define                                  //
// ========================================================================= //

#define FLAG_V 0x01
#define FLAG_D 0x02

#define PING_SENDING_DELAY 1
#define PING_MAX_PKT_SIZE 84
#define PING_PKT_SIZE 64


#define ICMP_PACKET_SUCCESS     0
#define ERR_ICMP_DEFAULT        -1
#define ERR_ICMP_TIME_EXCEEDED  -2
#define ERR_ICMP_NET_UNREACH    -3
#define ERR_ICMP_HOST_UNREACH   -4
#define ERR_ICMP_PROT_UNREACH   -5
#define ERR_ICMP_PORT_UNREACH   -6


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
    u8          flags;
    
    char        *hostname_in;
    char        hostname[INET6_ADDRSTRLEN];

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
bool reverse_dns(char *ip, char *hostname);
void prepare_packet(t_data *data, t_packet *packet, u16 n_sequence);
void send_packet(t_data *data, t_packet *packet, struct timeval *start_time, u16 *n_sequence);
void recv_packet(t_data *data, struct sockaddr_in *r_addr, socklen_t *addr_len, char *ip, u16 n_sequence, u16 *n_packet_received, struct timeval *start_time, struct timeval *end_time);


/* utils */
void    print_man();
void    print_header(t_data *data);
void    print_statistics(u16 n_sequence, u16 n_packet_received, struct timeval ping_start_time, struct timeval ping_end_time, char *hostname_in);

bool    manage_flags(i32 ac, char **av, u8 *flags);
u16     checksum(void *b, int len);
double  calcul_latency(struct timeval start_time, struct timeval end_time);


/* debug */
void print_recv_packet(t_packet *packet);
void print_sended_packet(t_packet *packet);
void print_received_packet(struct iphdr *ip_header, struct icmphdr *icmp_header, char *msg);

#endif /* INC_H */