/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:06:05 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/19 23:12:06 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*ft_calloc(size_t num_elements, size_t element_size)
{
	size_t	total_size;
	void	*ptr;

	if (num_elements && SIZE_MAX / num_elements < element_size)
		return (NULL);
	total_size = num_elements * element_size;
	ptr = malloc(total_size);
	if (!ptr)
		return (NULL);
	if (ptr)
		memset(ptr, 0, total_size);
	return (ptr);
}

long	get_current_time(void)
{
	struct timeval		tv;
	unsigned long long	time;

	if (gettimeofday(&tv, NULL) != 0)
		return (-1);
	time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time);
}

int	print_status(t_philosopher *philo, const char *status)
{
	long curry_time;

	if (philo == NULL || philo->data == NULL)
		return 1;
	pthread_mutex_lock(&philo->data->print_lock);
	pthread_mutex_lock(&philo->data->death);
	if (!philo->data->death_note || ft_strncmp(status, "died", 4) == 0) 
	{
		curry_time = get_current_time() - philo->data->start_time;
		if (curry_time == -1)
			return 1;
		printf("%ld %d %s\n", curry_time, philo->id, status);
	}
	pthread_mutex_unlock(&philo->data->death);
	pthread_mutex_unlock(&philo->data->print_lock);
	return 0;
}

int	ft_usleep(unsigned long long int time_value, t_philosopher *philo)
{
	unsigned long long	elapsed;
	struct timeval		start;
	struct timeval		current;

	if (philo == NULL || philo->data == NULL)
		return 1;
	if (gettimeofday(&start, NULL) == -1)
	{
		write(2, "Error: gettimeofday\n", 18);
		return 1;
	}
	while (1)
	{
		if (gettimeofday(&current, NULL) == -1)
		{
			write(2, "Error: gettimeofday\n", 18);
			return 1;
		}
		elapsed = (current.tv_sec - start.tv_sec) * 1000 + (current.tv_usec
				- start.tv_usec) / 1000;
		pthread_mutex_lock(&philo->data->death);
		if (elapsed >= time_value || philo->data->death_note == 1)
		{
			pthread_mutex_unlock(&philo->data->death);
			break ;
		}
		pthread_mutex_unlock(&philo->data->death);
		usleep(50);
	}
	return (0);
}
