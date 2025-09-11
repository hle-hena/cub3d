/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ternary.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 18:12:45 by hle-hena          #+#    #+#             */
/*   Updated: 2025/01/15 14:37:53 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

long long int	ft_tern_int(int cnd, long long int val1, long long int val2)
{
	if (cnd)
		return (val1);
	return (val2);
}

void	*ft_tern(int cnd, void *ret1, void *ret2)
{
	if (cnd)
		return (ret1);
	return (ret2);
}
