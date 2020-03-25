/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiskow <tiskow@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/06 11:21:31 by tiskow            #+#    #+#             */
/*   Updated: 2018/09/06 11:21:32 by tiskow           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/tcping.h"

void    handler(int sig)
{
    signal(sig, SIG_IGN);
    if (!stats.rcv)
        stats.min = 0;
    printf("\n\n--- %s tcping statistics ---\n", stats.host);
    printf("%.0f packets transmitted, %.0f received, %.2f%% packet loss\n", stats.trm, stats.rcv, ((stats.trm - stats.rcv) / stats.trm) * 100);
    printf("rtt min/avg/max = %.2f/%.2f/%.2f ms\n", stats.min, stats.avg, stats.max);
    exit(1);
}
