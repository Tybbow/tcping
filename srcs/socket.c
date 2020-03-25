/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiskow <tiskow@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/20 16:58:51 by tiskow            #+#    #+#             */
/*   Updated: 2018/08/24 16:40:24 by tiskow           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/tcping.h"

void	initPseudoHeader(t_mss **mss)
{
	(*mss)->psdheader.dst_addr = inet_addr((*mss)->addr_dst);
	(*mss)->psdheader.src_addr = inet_addr((*mss)->addr_src);
	(*mss)->psdheader.padding = 0;
	(*mss)->psdheader.proto = IPPROTO_TCP;
	(*mss)->psdheader.length = htons(sizeof(t_tcp) + sizeof(t_otcp));
    (*mss)->vmss->kind = 2;
    (*mss)->vmss->len = 4;
    (*mss)->vmss->mss = htons(1330);
}

void	initIP(t_mss **mss)
{
	(*mss)->iph->ihl = 5;
    (*mss)->iph->version = 4;
    (*mss)->iph->tos = (*mss)->qos;
    (*mss)->iph->tot_len = sizeof(struct ip) + sizeof(t_tcp);
    (*mss)->iph->id = htons (54321);
    (*mss)->iph->frag_off = htons(16384);
    (*mss)->iph->ttl = 224;
    (*mss)->iph->protocol = IPPROTO_TCP;
    (*mss)->iph->check = 0;
	(*mss)->iph->saddr = inet_addr((*mss)->addr_src);
    (*mss)->iph->daddr = inet_addr((*mss)->addr_dst);
	(*mss)->iph->check = csum((unsigned short *)(*mss)->datagram, (*mss)->iph->tot_len >> 1);
}

void	initTCP(t_mss **mss)
{
	(*mss)->tcph->source = htons(43219);
    (*mss)->tcph->dest = htons((*mss)->port);
    (*mss)->tcph->seq = htonl(12);
    (*mss)->tcph->ack_seq = 0;
    (*mss)->tcph->doff = sizeof(t_tcp) / 4 + 1;
    (*mss)->tcph->fin=0;
    (*mss)->tcph->syn=1;
    (*mss)->tcph->rst=0;
    (*mss)->tcph->psh=0;
    (*mss)->tcph->ack=0;
    (*mss)->tcph->urg=0;
    (*mss)->tcph->window = htons(65535);
    (*mss)->tcph->check = 0;
    (*mss)->tcph->urg_ptr = 0;
}

void	initMSS(t_mss **mss)
{
	initPseudoHeader(mss);
	initIP(mss);
	initTCP(mss);
    stats.min = 999999999;
    stats.max = 0;
}
