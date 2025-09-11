/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:19:58 by hle-hena          #+#    #+#             */
/*   Updated: 2024/12/10 13:19:22 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_calloc(size_t n, size_t size)
{
	void	*pt;

	if (size != 0 && n > (-1 / size))
		return (NULL);
	pt = malloc (n * size);
	if (!pt)
		return (NULL);
	ft_bzero(pt, n * size);
	return (pt);
}
