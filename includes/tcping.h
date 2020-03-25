/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcping.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiskow <tiskow@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/20 13:53:26 by tiskow            #+#    #+#             */
/*   Updated: 2018/11/04 14:24:22 by tiskow           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	TCPING_H
# define TCPING_H

# define COLOR_RED     "\x1b[31m"
# define COLOR_GREEN   "\x1b[32m"
# define COLOR_YELLOW  "\x1b[33m"
# define COLOR_BLUE    "\x1b[34m"
# define COLOR_MAGENTA "\x1b[35m"
# define COLOR_CYAN    "\x1b[36m"
# define COLOR_RESET   "\x1b[0m"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<errno.h>
#include<signal.h>
#include<pthread.h>
#include<unistd.h>
#include<netdb.h>
#include<ifaddrs.h>
#include<arpa/inet.h>
#include<netinet/tcp.h>
#include<netinet/ip.h>

typedef struct	s_mss 		t_mss;
typedef struct 	s_psdheader t_pshd;
typedef struct 	tcphdr		t_tcp;
typedef struct 	tcpopts     t_otcp;
typedef	struct	iphdr 		t_ip;
typedef struct  s_stats     t_stats;

struct      s_stats
{
    char    host[20];
    double  min;
    double  avg;
    double  max;
    double  trm;
    double  rcv;
};

struct tcpopts {
    uint8_t kind;
    uint8_t len;
    uint16_t mss;
} __attribute__((packed));

struct 		s_psdheader
{
	u_int32_t 	src_addr;
    u_int32_t	dst_addr;
    u_int8_t 	padding;
    u_int8_t 	proto;
    u_int16_t 	length;

	t_tcp 		tcp;
    t_otcp      otcp;
};

struct		s_mss
{
	char	 	datagram[4096];
	char 		addr_src[20];
	char 		addr_dst[20];
	int			qos;
	int			count;
	double 		interval;
	int			socket;
	double		timeout;
	int			port;

	t_tcp 		*tcph;
	t_ip 		*iph;
	t_otcp		*vmss;
	t_pshd		psdheader;
};


volatile    t_stats     stats;
int 	    getIPLocal(char **tmp);
double      time_diff(struct timeval x , struct timeval y);
void	    start_thread(t_mss *mss, size_t count);
void        statistics(double value);
void	    initMSS(t_mss **mss);
int		    receive_ack(t_mss *mss);
void	    sendSyn(t_mss *mss);
void	    findMss(t_mss **mss, size_t value);
u_short     csum(u_short *ptr, int nbytes);
void        handler(int sig);
#endif
