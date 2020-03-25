/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sniffer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiskow <tiskow@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 11:42:03 by tiskow            #+#    #+#             */
/*   Updated: 2018/09/06 08:48:42 by tiskow           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/tcping.h"

int		process_packet(unsigned char *buffer, t_mss *mss)
{
	struct iphdr *iph = (struct iphdr *)buffer;
	size_t iphdrlen;

	if (iph->protocol == 6)
	{
		iphdrlen = iph->ihl*4;
		struct tcphdr *tcph = (struct tcphdr *)(buffer + iphdrlen);
		if (tcph->syn == 1 && tcph->ack == 1 && iph->saddr == inet_addr(mss->addr_dst) && tcph->dest == mss->tcph->source)
		{
			fflush(stdout);
			return (1);
		}
	}
	return (0);
}

int		start_sniffer(t_mss *mss)
{
	int sock_raw;
	socklen_t saddr_size;
	int data_size;
	struct sockaddr saddr;
	u_char buffer[4096];

	fflush(stdout);
	sock_raw = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
	if (sock_raw < 0)
	{
		fflush(stdout);
		exit(1);
	}
	saddr_size = sizeof(saddr);
	while (1)
	{
		data_size = recvfrom(sock_raw, buffer, 4096, 0, &saddr, &saddr_size);
		if (data_size >= 0)
		{
			if (process_packet(buffer, mss))
				return (1);
		}
	}
	return (0);
}

int		receive_ack(t_mss *mss)
{
	if(start_sniffer(mss))
		return (1);
	return (0);
}

