/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_init_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 22:34:45 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/20 22:38:49 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	initialize_mutex(pthread_mutex_t *mutex)
{
	if (pthread_mutex_init(mutex, NULL) != 0)
		return (1);
	return (0);
}

int	destroy_fork_mutexes(t_data *data, int i)
{
	i--;
	while (i >= 0)
	{
		pthread_mutex_destroy(&data->fork[i]);
		i--;
	}
	free(data->fork);
	free(data->philosophers);
	free(data);
	data = NULL;
	return (1);
}
