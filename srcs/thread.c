/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiskow <tiskow@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/20 16:06:21 by tiskow            #+#    #+#             */
/*   Updated: 2018/09/06 09:16:25 by tiskow           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/tcping.h"

pthread_t myThreadSniffer;
pthread_t myThreadSendSyn;
pthread_t myThreadSleep;

void	*threadSniffer(void *arg)
{
	struct timeval *after;
	t_mss *mss = (t_mss*)arg;

	if (receive_ack(mss))
		pthread_cancel(myThreadSleep);
	after = malloc(sizeof(struct timeval));
	gettimeofday(after, NULL);
    stats.rcv++;
	pthread_exit(after);
}

void	*threadSendSyn(void *arg)
{
	struct timeval *before;
	t_mss *mss = (t_mss*)arg;

	before = malloc(sizeof(struct timeval));
	gettimeofday(before, NULL);
	sendSyn(mss);
    stats.trm++;
	pthread_exit(before);
}

void	*threadSleep(void *arg)
{
	int *ret;
	t_mss *mss = (t_mss *)arg;

	sleep(mss->timeout);
	ret = malloc(sizeof(int));
	*ret = 42;
	pthread_cancel(myThreadSniffer);
	pthread_exit((void *)&ret);
}

void	start_thread(t_mss *mss, size_t count)
{
	struct 	timeval *after, *before;
	void 	*status;
    double  time;

	after = NULL;
	before = NULL;
	pthread_create(&myThreadSleep, NULL, threadSleep, mss);
	pthread_create(&myThreadSendSyn, NULL, threadSendSyn, mss);
	pthread_create(&myThreadSniffer, NULL, threadSniffer, mss);

	pthread_join(myThreadSleep, &status);
	pthread_join(myThreadSniffer, (void **)&after);
	pthread_join(myThreadSendSyn, (void **)&before);

	if (*(int *)&status != -1)
    {
        free(before);
		printf("timeout...\n");
    }
	else
    {
        time = time_diff(*before, *after);
        statistics(time);
		printf("from (%s): tcp_seq=%lu time=%.2f ms\n",mss->addr_dst, count, time);
        free(before);
        free(after);
    }
}

