/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 12:08:29 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/06 17:05:35 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline float	to_linear(float c)
{
	return ((c / 255.0f) * (c / 255.0f));
}

static inline float	to_srgb(float c)
{
	return (sqrtf(c) * 255.0f);
}

int	add_col_val_physical(int col1, int col2, float weight1,
	float weight2)
{
	t_col	final;

	final.re = (int)to_srgb((weight1 * to_linear((col1 >> 16) & 0xFF))
			+ (to_linear((col2 >> 16) & 0xFF) * weight2));
	final.gr = (int)to_srgb((weight1 * to_linear((col1 >> 8) & 0xFF))
			+ (to_linear((col2 >> 8) & 0xFF) * weight2));
	final.bl = (int)to_srgb((weight1 * to_linear(col1 & 0xFF))
			+ (to_linear(col2 & 0xFF) * weight2));
	if (final.re > 255)
		final.re = 255;
	if (final.gr > 255)
		final.gr = 255;
	if (final.bl > 255)
		final.bl = 255;
	return ((final.re << 16) | (final.gr << 8) | final.bl);
}

void	raytrace_source(t_data *data, t_light light)
{
	float	deg;
	t_vec	dir;

	deg = 0;
	printf("Doing (%.2f, %.2f) for %.2f\n", light.pos.x,
		light.pos.y, light.emittance);
	while (deg < 360)
	{
		dir.x = cos(deg * PI / 180);
		dir.y = sin(deg * PI / 180);
		raytrace(data, light, dir);
		deg += 0.002;
	}
}

int	create_lmap(t_data *data)
{
	int			i;
	t_lmap		*lmap;

	lmap = &data->lmap;
	lmap->wid = data->map->wid * LMAP_PRECISION;
	lmap->len = data->map->len * LMAP_PRECISION;
	data->lmap.lmap = ft_calloc(lmap->wid * lmap->len, sizeof(t_tlight));
	if (!lmap->lmap)
		return (1);
	if (!lmap->lights)
		return (0);
	i = -1;
	while (++i < lmap->nb_ls)
	{
		lmap->lights[i].index = i + 1;
		raytrace_source(data, lmap->lights[i]);
	}
	return (0);
}
