/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@students.42perpignan    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 09:54:50 by hle-hena          #+#    #+#             */
/*   Updated: 2024/11/18 18:06:02 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strnstr(const char *str, const char *to_find, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (to_find[0] == '\0')
		return ((char *) str);
	while (i < len && str[i])
	{
		j = 0;
		while (i + j < len && str[i + j] == to_find[j] && to_find[j])
			j ++;
		if (to_find[j] == '\0')
			return ((char *)&str[i]);
		i ++;
	}
	return (NULL);
}
