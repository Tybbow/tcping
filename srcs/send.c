/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiskow <tiskow@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 15:05:09 by tiskow            #+#    #+#             */
/*   Updated: 2018/09/06 08:46:36 by tiskow           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/tcping.h"

void	sendSyn(t_mss *mss)
{
	struct sockaddr_in  dst;

    bzero((char *) &dst, sizeof(dst));
	dst.sin_family = AF_INET;
	dst.sin_addr.s_addr = inet_addr(mss->addr_dst);
    dst.sin_port = htons(mss->port);
    mss->tcph->check = 0;

	memcpy(&mss->psdheader.tcp, mss->tcph, sizeof(t_pshd));
	mss->tcph->check = csum((u_short *)&mss->psdheader, sizeof(t_pshd));
	sendto(mss->socket, mss->datagram, sizeof(t_ip) + sizeof(t_tcp) + sizeof(t_otcp), 0, (struct sockaddr *)&dst, sizeof(dst));
}
