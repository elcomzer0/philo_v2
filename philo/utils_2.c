/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 22:25:59 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/19 18:36:57 by jorgonca         ###   ########.fr       */
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
		return (0);
	i = 0;
	if (n == 0)
		return (0);
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
		return (0);
	sign = 1;
	n = 0;
	i = 0;
	skip_spaces_and_sign(str, &sign, &i);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (n > INT_MAX / 10 || (n == INT_MAX / 10 && (str[i] - '0') > INT_MAX
				% 10))
		{
			if (sign == -1)
				return (INT_MIN);
			else
				return (INT_MAX);
		}
		n = 10 * n + (str[i] - '0');
		i++;
	}
	return (n * sign);
}

long	ft_atol(const char *str)
{
	int		sign;
	int		i;
	long	n;

	if (!str)
		return (0);
	sign = 1;
	n = 0;
	i = 0;
	skip_spaces_and_sign(str, &sign, &i);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (n > LONG_MAX / 10 || (n == LONG_MAX / 10 && str[i] - '0' > LONG_MAX
				% 10))
		{
			if (sign == -1)
				return (LONG_MIN);
			else
				return (LONG_MAX);
		}
		n = 10 * n + (str[i] - '0');
		i++;
	}
	return (n * sign);
}
