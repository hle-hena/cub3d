/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@students.42perpignan    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:53:33 by hle-hena          #+#    #+#             */
/*   Updated: 2024/11/18 18:06:10 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_substr(char const *src, unsigned int start, size_t len)
{
	char	*dest;
	size_t	strlen;
	size_t	i;

	strlen = 0;
	while (src[strlen])
		strlen++;
	if (start >= strlen)
		return (ft_calloc(1, sizeof(char)));
	if (len > strlen - start)
		len = strlen - start;
	dest = ft_calloc(len + 1, sizeof(char));
	if (!dest)
		return (0);
	i = -1;
	while (++i < len)
		dest[i] = src[i + start];
	return (dest);
}
