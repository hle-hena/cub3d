/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@students.42perpignan    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 09:27:47 by hle-hena          #+#    #+#             */
/*   Updated: 2024/11/18 18:04:33 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_memset(void *add, int oct, size_t n)
{
	while (n > 0)
	{
		n--;
		((unsigned char *)add)[n] = (unsigned char)oct;
	}
	return (add);
}
