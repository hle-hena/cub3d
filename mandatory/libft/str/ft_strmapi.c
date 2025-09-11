/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@students.42perpignan    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 13:57:20 by hle-hena          #+#    #+#             */
/*   Updated: 2024/11/18 18:05:52 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strmapi(const char *str, char (*f)(unsigned int, char))
{
	size_t	i;
	char	*dest;

	dest = ft_strdup(str);
	if (!dest)
		return (NULL);
	i = -1;
	while (dest[++i])
		dest[i] = f(i, dest[i]);
	return (dest);
}
