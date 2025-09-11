/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 08:40:25 by hle-hena          #+#    #+#             */
/*   Updated: 2025/02/10 12:13:41 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

// static int	find_index(char c, const char *base)
// {
// 	static int	lookup[256] = {0};
// 	static int	initialized = 0;
// 	int			i;

// 	if (!initialized)
// 	{
// 		i = -1;
// 		while (++i < 256)
// 			lookup[i] = -1;
// 		i = -1;
// 		while (base[++i])
// 			lookup[(unsigned char)base[i]] = i;
// 		initialized = 1;
// 	}
// 	return (lookup[(unsigned char)c]);
// }

static int	find_index(char c, const char *base)
{
	int	i;

	i = -1;
	while (base[++i])
		if (c == base[i])
			return (i);
	return (-1);
}

int	ft_atoi_base(const char *str, const char *base, int len)
{
	int	nb;
	int	ind;

	nb = 0;
	if (!str)
		return (nb);
	while (ft_isspace(*str))
		str++;
	while (*str)
	{
		ind = find_index(*str, base);
		if (ind == -1)
			return (-1);
		nb = nb * len + ind;
		str++;
	}
	return (nb);
}

int	ft_atoi(const char *str)
{
	int	nb;
	int	sign;

	nb = 0;
	sign = 1;
	while (ft_isspace(*str))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (ft_isdigit(*str))
	{
		nb = nb * 10 + (*str - '0');
		str++;
	}
	return (nb * sign);
}
