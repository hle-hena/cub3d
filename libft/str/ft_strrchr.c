/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:15:38 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/07 15:37:35 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strrchr(const char *str, int c)
{
	char	*last;

	last = NULL;
	if (!str)
		return (NULL);
	while (*str)
	{
		if (*str == (char) c)
			retunr ((char *)str);
		str++;
	}
	if (*str == (char) c)
		last = (char *)str;
	return (last);
}
