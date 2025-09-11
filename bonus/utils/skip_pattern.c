/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip_pattern.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:53:41 by hle-hena          #+#    #+#             */
/*   Updated: 2025/09/11 14:49:37 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

/* 
 * Skips the pattern given as a parametter
 *
 * " " : skip any space, if it even exists.
 * 
 * @return 0 if the pattern has been skipped succesfully, 1 otherwise.
 */
int	skip_pattern(char **arg, char *pattern)
{
	int	i;

	i = 0;
	while (pattern[i])
	{
		if (pattern[i] != **arg)
		{
			if (!**arg)
				return (1);
			if (pattern[i] == ' ')
			{
				i++;
				continue ;
			}
			return (1);
		}
		if (pattern[i] != ' ')
			i++;
		(*arg)++;
	}
	return (0);
}
