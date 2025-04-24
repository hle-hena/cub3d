/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:49:06 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/24 15:25:11 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

float	ft_atof(const char *str)
{
	float	nb;
	int		sign;
	int		div;

	nb = 0;
	sign = 1;
	while (ft_isspace(*str))
		str++;
	if (*str == '+' || *str == '-')
	{
		sign = -1 * (*str == '-') + 1 * (*str != '-');
		str++;
	}
	while (ft_isdigit(*str))
		nb = nb * 10 + (*str++ - '0');
	if (*str == '.')
	{
		div = 10;
		while (ft_isdigit(*str++))
		{
			nb += (*str - '0') / div;
			div *= 10;
		}
	}
	return (nb * sign);
}
