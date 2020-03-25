/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiskow <tiskow@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/20 15:43:49 by tiskow            #+#    #+#             */
/*   Updated: 2018/09/06 08:45:38 by tiskow           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/tcping.h"

int	    getIPLocal(char **tmp)
{
	struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == -1)
        return (0);
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr != NULL && ifa->ifa_addr->sa_family == AF_INET)
        {
            if (!strcmp(ifa->ifa_name, "en0") || !strcmp(ifa->ifa_name, "eth0") || 
            !strcmp(ifa->ifa_name, "enp0s3"))
            {
                strcpy(*tmp, inet_ntoa(((struct sockaddr_in *)ifa->ifa_addr)->sin_addr));
                freeifaddrs(ifaddr);
                return (1);
            }
        }
    }
    return (0);
}

unsigned short csum(unsigned short *ptr, int nbytes) 
{
    register long sum;
    unsigned short oddbyte;
    register short answer;
 
    sum=0;
    while(nbytes>1) {
        sum+=*ptr++;
        nbytes-=2;
    }
    if(nbytes==1) {
        oddbyte=0;
        *((unsigned char *)&oddbyte)=*(unsigned char *)ptr;
        sum+=oddbyte;
    }
 
    sum = (sum>>16)+(sum & 0xffff);
    sum = sum + (sum>>16);
    answer=(short)~sum;
     
    return(answer);
}

void    statistics(double value)
{
    if (stats.min > value)
        stats.min = value;
    if (stats.max < value)
        stats.max = value;
    stats.avg = (stats.avg * (stats.trm - 1) + value) / stats.trm;
}

double  time_diff(struct timeval x , struct timeval y)
{
    double x_ms , y_ms , diff;
         
    x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
    y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;                  
    diff = (double)y_ms - (double)x_ms;
    return ((diff / 1000));
}
