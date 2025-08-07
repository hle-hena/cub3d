/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retrieve_texture_val.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:19:48 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/07 15:49:03 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

float	ft_atof_err(char *str, float min, float max, char **last)
{
	float	nb;
	int		div;

	nb = 0;
	while (ft_isspace(*str))
		str++;
	while (ft_isdigit(*str))
	{
		nb = nb * 10 + (*str++ - '0');
		if (nb > max && max > 0)
			return (*last = str, -1);
	}
	if (*str == '.')
	{
		div = 10;
		while (ft_isdigit(*++str))
		{
			nb += (float)(*str - '0') / div;
			div *= 10;
			if (nb > max && max > 0)
				return (*last = str, -1);
		}
	}
	*last = str;
	return ((nb < min) * -1 + (!(nb < min)) * nb);
}

void	retrieve_texture_val(t_text *texture, char *arg, int *err)
{
	char	*last;

	texture->reflectance = ft_atof_err(arg, 0, 1, &last);
	if (texture->reflectance == -1)
		return (ft_perror(-1, "Expected a postive number smaller than 1.", 0),
			*err = 1, VOID);
	if (*last != 0 && *last != '\n')
		return (ft_perror(-1, "Expected end of line after reflectance.", 0),
			*err = 1, VOID);
}
