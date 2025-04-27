/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 12:08:29 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/27 19:37:25 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	raytrace_source(t_data *data, t_vec origin)
{
	float	deg;
	t_vec	dir;

	deg = 0;
	while (deg < 360)
	{
		dir.x = cos(deg * PI / 180);
		dir.y = sin(deg * PI / 180);
		raytrace(data, origin, dir, 1);
		deg += 0.01;
	}
}

int	create_lmap(t_data *data)
{
	int		i;
	t_lmap	*lmap;

	lmap = &data->lmap;
	data->lmap.lmap = ft_calloc(data->map->wid * data->map->len * LMAP_PRECISION * LMAP_PRECISION, sizeof(float));
	if (!lmap->lmap)
		return (1);
	lmap->nb_l_sources = 1;
	lmap->light_sources = malloc(sizeof(t_vec));
	lmap->light_sources[0] = (t_vec){1.5, 1.5};
	i = -1;
	while (++i < lmap->nb_l_sources)
		raytrace_source(data, lmap->light_sources[i]);
	return (0);
}
