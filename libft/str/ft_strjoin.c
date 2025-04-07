/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:04:59 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/04 15:56:48 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*dest;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	dest = ft_calloc(s1_len + s2_len + 1, sizeof(char));
	if (!dest)
		return (NULL);
	ft_strlcpy(dest, s1, s1_len + 1);
	ft_strlcat(dest, s2, s1_len + s2_len + 1);
	return (dest);
}

char	*ft_strsjoin(const char **strs)
{
	char	*dest;
	int		i;
	size_t	len;

	i = 0;
	len = 0;
	while (strs[i])
		len += ft_strlen(strs[i++]);
	dest = ft_calloc((len + 1), sizeof(char));
	if (!dest)
		return (NULL);
	i = 0;
	len = 0;
	while (strs[i])
	{
		if (strs[i][0])
		{
			ft_strlcpy(dest + len, strs[i], ft_strlen(strs[i]) + 1);
			len += ft_strlen(strs[i]);
		}
		i++;
	}
	return (dest);
}
