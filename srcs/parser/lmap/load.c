/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 12:08:29 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/28 13:52:24 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	add_lmap(t_data *data, float **final, float *temp)
{
	int		x;
	int		y;
	int		i;

	y = -1;
	i = 0;
	while (++y < data->map->len * LMAP_PRECISION)
	{
		x = -1;
		while (++x < data->map->wid * LMAP_PRECISION)
		{
			*(*final + i) = *(*final + i) + *(temp + i);
			if (*(*final + i) > 1)
				*(*final + i) = 1;
			i++;
		}
	}
}

void	raytrace_source(t_data *data, t_light light)
{
	float	deg;
	t_vec	dir;

	deg = 0;
	printf("Doing (%.2f, %.2f) for %.2f\n", light.pos.x, light.pos.y, light.emittance);
	while (deg < 360)
	{
		dir.x = cos(deg * PI / 180);
		dir.y = sin(deg * PI / 180);
		raytrace(data, light.pos, dir, light.emittance);
		deg += 0.01;
	}
}

int	create_lmap(t_data *data)
{
	int		i;
	t_lmap	*lmap;
	float	*temp;

	lmap = &data->lmap;
	data->lmap.lmap = ft_calloc(data->map->wid * data->map->len * LMAP_PRECISION
		* LMAP_PRECISION, sizeof(float));
	temp = ft_calloc(data->map->wid * data->map->len * LMAP_PRECISION
		* LMAP_PRECISION, sizeof(float));
	if (!lmap->lmap || !temp)
		return (1);
	if (!lmap->lights)
		return (0);
	i = -1;
	while (++i < lmap->nb_ls)
	{
		raytrace_source(data, lmap->lights[i]);
		add_lmap(data, &temp, data->lmap.lmap);
		ft_bzero(data->lmap.lmap, data->map->wid * data->map->len
			* LMAP_PRECISION * LMAP_PRECISION * sizeof(float));
	}
	ft_del((void **)&data->lmap.lmap);
	data->lmap.lmap = temp;
	return (0);
}
