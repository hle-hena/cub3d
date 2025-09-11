/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@students.42perpignan    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 09:18:25 by hle-hena          #+#    #+#             */
/*   Updated: 2024/11/18 18:04:26 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_memcmp(const void *add1, const void *add2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && ((unsigned char *)add1)[i] == ((unsigned char *)add2)[i])
		i ++;
	if (i == n)
		return (0);
	return (((unsigned char *)add1)[i] - ((unsigned char *)add2)[i]);
}
