/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_max.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 14:19:06 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/15 20:32:06 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

inline int	ft_max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

inline int	ft_min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}
