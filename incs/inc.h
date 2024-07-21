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
#include <math.h>
#include <errno.h>


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
//                                   Macro                                   //
// ========================================================================= //

#define __ip_str(addr) inet_ntoa(*(in_addr *)&(addr))
#define __log_error(error) (void)fprintf(stderr, "%s: %s\n", error, strerror(errno))


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

typedef struct sockaddr_in  sockaddr_in;
typedef struct sockaddr     sockaddr;
typedef struct addrinfo     addrinfo;
typedef struct in_addr      in_addr;
typedef struct icmphdr      icmphdr;
typedef struct iphdr        iphdr;
typedef struct timeval      timeval;


// ========================================================================= //
//                                  Structure                                //
// ========================================================================= //

typedef struct {
    i32 sockfd;
    u16 pid;
    u8  flags;
    
    char    *hostname_in;
    char    hostname[INET6_ADDRSTRLEN];

    sockaddr_in dest;
} t_data;

typedef struct {
    icmphdr hdr;
    char    payload[PING_PKT_SIZE - sizeof(icmphdr)];
} t_packet;

typedef struct {
    double  *arr;
    u32     size;
} t_times;


// ========================================================================= //
//                                  Prototype                                //
// ========================================================================= //

/* core */
bool initialization(t_data *data);
bool reverse_dns(char *ip, char *hostname);

void prepare_packet(t_data *data, t_packet *packet, u16 n_sequence);
void send_packet(t_data *data, t_packet *packet, timeval *start_time,
                    u16 *n_sequence);
void recv_packet(t_data *data, sockaddr_in *r_addr, socklen_t *addr_len,
                    u16 n_sequence, u16 *n_packet_received, timeval *start_time,
                    timeval *end_time, t_times *times);

/* utils */
void print_man(void);
void print_header(t_data *data);
void print_statistics(u16 n_sequence, u16 n_packet_received, char *hostname_in,
                        t_times *times);
void print_verbose_option(iphdr *ip_hdr, icmphdr *icmp_hdr);

bool manage_flags(i32 ac, char **av, u8 *flags);
u16 checksum(void *b, int len);

double calcul_latency(timeval start_time, timeval end_time);
void calcul_statistics(t_times *times, double *min, double *max,
                        double *avg, double *stddev);

void realloc_push_time(t_times *times, double time);
void free_times(t_times *times);
void close_sockfd_and_exit(t_data *data);

bool ip_to_hostname(const char *ip, char *res);

/* debug */
void print_sent_packet(t_packet *packet);
void print_received_packet(iphdr *ip_header, icmphdr *icmp_header,
                            char *payload);


#endif /* INC_H */