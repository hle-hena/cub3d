/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:49:27 by hle-hena          #+#    #+#             */
/*   Updated: 2024/12/11 12:21:51 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	count_words(const char *str, char c)
{
	int		s;
	int		e;
	int		i;

	i = 0;
	s = 0;
	e = 0;
	while (str[e])
	{
		while (str[e] && str[e] == c)
			e ++;
		s = e;
		while (str[e] && str[e] != c)
			e ++;
		if (s < e)
			i ++;
	}
	return (i);
}

static void	freeing_splited(char **splited, int i)
{
	while (i >= 0)
	{
		free(splited[i]);
		i --;
	}
	free(splited);
}

char	**ft_split(const char *str, char c)
{
	char	**splited;
	int		s;
	int		e;
	int		i;

	i = 0;
	e = 0;
	splited = ft_calloc(count_words(str, c) + 1, sizeof(char *));
	if (!splited)
		return (NULL);
	while (str[e])
	{
		while (str[e] && str[e] == c)
			e++;
		s = e;
		while (str[e] && str[e] != c)
			e++;
		if (s < e)
		{
			splited[i++] = ft_substr(str, s, e - s);
			if (!splited[i - 1])
				return (freeing_splited(splited, i - 1), NULL);
		}
	}
	return (splited);
}
