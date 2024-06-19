/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:31 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/19 16:14:42 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philosopher	*allocate_philosophers(int number_of_philosophers)
{
	t_philosopher	*philosophers;

	philosophers = (t_philosopher *)ft_calloc(number_of_philosophers,
			sizeof(t_philosopher));
	if (!philosophers)
	{
		write(2, "Error allocating philosophers\n", 35);
		return (NULL);
	}
	return (philosophers);
}

void	assign_forks(t_philosopher *philosopher, t_data *data)
{
	if (philosopher->id % 2 == 1)
	{
		philosopher->right_fork = &data->fork[philosopher->id - 1];
		philosopher->left_fork = &data->fork[philosopher->id
			% data->number_of_philosophers];
	}
	else
	{
		philosopher->right_fork = &data->fork[philosopher->id
			% data->number_of_philosophers];
		philosopher->left_fork = &data->fork[philosopher->id - 1];
	}
}

int	init_data(t_data *data, int argc, char **argv)
{
	if (validate_argument(argv[1], "Error: invalid number of philosophers\n")
		|| validate_argument(argv[2], "Error: invalid time_to_die\n")
		|| validate_argument(argv[3], "Error: invalid time_to_eat\n")
		|| validate_argument(argv[4], "Error: invalid time_to_sleep\n"))
		return (1);
	data->number_of_philosophers = ft_atoi(argv[1]);
	if (validate_number_of_philosophers(data->number_of_philosophers))
		return (1);
	data->time_to_die = ft_atol(argv[2]);
	data->time_to_eat = ft_atol(argv[3]);
	data->time_to_sleep = ft_atol(argv[4]);
	if (argc == 6)
	{
		if (validate_argument(argv[5],
				"Error: invalid argument for times_must_eat\n"))
			return (1);
		data->times_must_eat = ft_atoi(argv[5]);
		if (validate_times_must_eat(data->times_must_eat))
			return (1);
	}
	else
		data->times_must_eat = -1;
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	*data;

	data = ft_calloc(1, sizeof(t_data));
	if (!data)
	{
		write(2, "Error allocating data\n", 22);
		return (1);
	}
	if (argc != 5 && argc != 6)
	{
		write(STDERR_FILENO, "Usage: ./philo number_of_philosophers", 35);
		write(STDERR_FILENO,
			"time_to_die time_to_eat time_to_sleep [times_must_eat]\n", 37);
		return (1);
	}
	if (init_data(data, argc, argv) != 0)
	{
		write(STDERR_FILENO, "Error: invalid arguments\n", 25);
		return (1);
	}
	initialize_philosophers(data);
	//if (data->number_of_philosophers > 1)
		create_threads(data);
	clean_exit(data);
	return (0);
}
