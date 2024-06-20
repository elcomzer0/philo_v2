/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 14:27:34 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/20 14:30:52 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	gettimeofday_protected(struct timeval *tv)
{
	if (gettimeofday(tv, NULL) == -1)
	{
		write(2, "Error: gettimeofday\n", 18);
		return (1);
	}
	return (0);
}

int	usleep_wrapper(void)
{
	if (usleep(50) == -1)
	{
		write(2, "Error: usleep\n", 13);
		return (1);
	}
	return (0);
}

int	ft_usleep(unsigned long long int time_value, t_philosopher *philo)
{
	unsigned long long	elapsed;
	struct timeval		start;
	struct timeval		current;

	if (philo == NULL || philo->data == NULL)
		return (1);
	if (gettimeofday_protected(&start) == 1)
		return (1);
	while (1)
	{
		if (gettimeofday_protected(&current) == 1)
			return (1);
		elapsed = (current.tv_sec - start.tv_sec) * 1000 + (current.tv_usec
				- start.tv_usec) / 1000;
		pthread_mutex_lock(&philo->data->death);
		if (elapsed >= time_value || philo->data->death_note == 1)
		{
			pthread_mutex_unlock(&philo->data->death);
			break ;
		}
		pthread_mutex_unlock(&philo->data->death);
		if (usleep_wrapper() == 1)
			return (1);
	}
	return (0);
}
