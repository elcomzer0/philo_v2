/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 22:25:59 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/20 22:32:49 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (!s1 || !s2)
		return (-1);
	i = 0;
	if (n == 0)
		return (-1);
	while (s1[i] && s2[i] && s1[i] == s2[i] && i < n - 1)
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	skip_spaces_and_sign(const char *str, int *sign, int *i)
{
	while ((str[*i] >= 9 && str[*i] <= 13) || str[*i] == 32)
		(*i)++;
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			*sign = -1 * *sign;
		(*i)++;
	}
	return (*i);
}

int	ft_atoi(const char *str)
{
	int		sign;
	int		i;
	long	n;

	if (!str)
		return (-1);
	sign = 1;
	n = 0;
	i = 0;
	skip_spaces_and_sign(str, &sign, &i);
	if (sign < 0)
		return (-1);
	while (str[i] >= '0' && str[i] <= '9')
	{
		n = 10 * n + (str[i] - '0');
		i++;
		if (n > INT_MAX)
			return (-1);
	}
	return ((int)n);
}

long	ft_atol(const char *str)
{
	int			sign;
	int			i;
	long long	n;

	if (!str)
		return (-1);
	sign = 1;
	n = 0;
	i = 0;
	skip_spaces_and_sign(str, &sign, &i);
	if (sign < 0)
		return (-1);
	while (str[i] >= '0' && str[i] <= '9')
	{
		n = 10 * n + (str[i] - '0');
		i++;
		if (n > LONG_MAX)
			return (-1);
	}
	return ((long)n);
}
