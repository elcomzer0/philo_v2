/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_valid.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 21:34:06 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/19 00:04:55 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_valid_number(char *str)
{
	if (*str == '0' && *(str + 1) != '\0')
		return (0);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}

int	is_within_limits(char *str)
{
	long	value;

	value = ft_atol(str);
	if (value <= 0 || value > INT_MAX)
		return (0);
	return (1);
}

int	validate_argument(char *arg, const char *error_message)
{
	if (!is_valid_number(arg) || !is_within_limits(arg))
	{
		write(STDERR_FILENO, error_message, ft_strlen(error_message));
		return (1);
	}
	return (0);
}

int	validate_number_of_philosophers(int number_of_philosophers)
{
	if (number_of_philosophers > MAX_PHILOSOPHERS)
	{
		write(STDERR_FILENO,
			"Error: number of philosophers exceeds the maximum limit\n", 56);
		return (1);
	}
	return (0);
}

int	validate_times_must_eat(int times_must_eat)
{
	if (times_must_eat <= 0)
	{
		write(STDERR_FILENO,
			"Error: times_must_eat must be a positive integer\n", 49);
		return (1);
	}
	return (0);
}
