/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiskow <tiskow@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 09:41:27 by tiskow            #+#    #+#             */
/*   Updated: 2018/09/06 16:58:10 by tiskow           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/tcping.h"

int     usage()
{
    printf("Usage :\t./tcping [-d destination addr] [-s source addr]\n");
    printf("\t\t  [-t timeout] [-c count] [-p port] [-Q tos]\n");
    printf("\t\t  [-i interval]\n");
    printf("\n");
    return (0);
}

int		checkOpt(int ac, char **av, t_mss **mss)
{
	int i;
	
	i = 1;
	(*mss)->timeout = 3;
	(*mss)->port = 80;
    (*mss)->interval = 1;
    (*mss)->count = -1;
    char *tmp = (*mss)->addr_src;
    if (!getIPLocal(&tmp))
    {
        printf(COLOR_RED "[-] " COLOR_RESET "No interface found.. Please use -s\n");
        return (0);
    }
	memset((*mss)->addr_dst, 0, 20);
	while (i < ac)
	{
		if (!strcmp(av[i], "-s"))
			strcpy((*mss)->addr_src, av[i + 1]);
		if (!strcmp(av[i], "-d"))
			strcpy((*mss)->addr_dst, av[i + 1]);
		if (!strcmp(av[i], "-t"))
			(*mss)->timeout = atoi(av[i + 1]);
		if (!strcmp(av[i], "-c"))
			(*mss)->count = atoi(av[i + 1]);
		if (!strcmp(av[i], "-Q"))
			(*mss)->qos = atoi(av[i + 1]);
		if (!strcmp(av[i], "-p"))
			(*mss)->port = atoi(av[i + 1]);
		if (!strcmp(av[i], "-i"))
			(*mss)->interval = atof(av[i + 1]);
        if (!strcmp(av[i], "-h"))
            return (0);
		i++; 
	}
	if (strlen((*mss)->addr_dst) > 0)
		return (1);
	return (0);
}

void	launch_checkMSS(t_mss **mss)
{
	int one = 1;
	size_t seq = 1;
    const int *val = &one;
	(*mss)->socket = socket(AF_INET, SOCK_RAW , IPPROTO_TCP);
    if((*mss)->socket < 0)
    {
        printf (COLOR_RED "[-] " COLOR_RESET "Error creating socket. Error number : %d. \n\tError message : %s \n" , errno , strerror(errno));
        exit(0);
    }
    if (setsockopt ((*mss)->socket, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
    {
        printf ("Error setting IP_HDRINCL. Error number : %d . Error message : %s \n" , errno , strerror(errno));
        exit(0);
    }
    strcpy((char *)stats.host, (*mss)->addr_dst);
	initMSS(mss);
    signal(SIGINT, (void (*)(int))handler);
	while ((*mss)->count)
	{
		start_thread(*mss, seq);
		usleep((*mss)->interval * 1000000);
		if ((*mss)->count > 0)
            (*mss)->count--;
        seq++;
	}
    free(*mss);
    handler(SIGINT);
}

t_mss	*initStruct()
{
	t_mss *mss;

	mss = NULL;
	mss = (t_mss *)malloc(sizeof(t_mss));
	if (!mss)
		return (NULL);
	memset(mss->datagram, 0, 4096);
	mss->iph = (t_ip *)mss->datagram;
	mss->tcph = (t_tcp *)(mss->datagram + sizeof(t_ip));
    mss->vmss = (t_otcp *)(mss->datagram + sizeof(t_ip) + sizeof(t_tcp));
	return (mss);
}

int		main(int ac, char **av)
{
	t_mss 	*mss;

	mss = initStruct();
	if(mss && checkOpt(ac, av, &mss))
		launch_checkMSS(&mss);
	else
		return (usage());
	return (0);

}
