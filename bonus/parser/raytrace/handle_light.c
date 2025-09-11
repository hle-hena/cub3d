/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_light.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:32:35 by hle-hena          #+#    #+#             */
/*   Updated: 2025/09/11 14:50:43 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

static inline void	test_value(t_point curr, float dir, int *is_out,
	int *should_stop)
{
	if (curr.x < 0)
	{
		*is_out = 1;
		if (dir <= 0)
			*should_stop = 1;
	}
	else if (curr.x >= curr.y)
	{
		*is_out = 1;
		if (dir >= 0)
			*should_stop = 1;
	}
}

static inline int	is_outside(t_data *data, t_trace *ray)
{
	int	is_out;
	int	should_stop;

	is_out = 0;
	should_stop = 0;
	test_value((t_point){ray->curr.x, data->lmap.wid},
		ray->dir.x, &is_out, &should_stop);
	test_value((t_point){ray->curr.y, data->lmap.len},
		ray->dir.y, &is_out, &should_stop);
	if (should_stop)
		ray->running = 0;
	return (is_out);
}

void	handle_light(t_data *data, t_trace *ray, t_light light)
{
	t_tile		*tile;
	t_wpath		wall;

	if (is_outside(data, ray))
		return ;
	ray->real.x = ray->curr.x / LMAP_PRECISION;
	ray->real.y = ray->curr.y / LMAP_PRECISION;
	tile = get_tile_dict()[*(data->map->map + ray->real.y * data->map->wid
			+ ray->real.x)];
	if (!tile)
		return ;
	light_floor(data, ray, light);
	if (!tile->is_wall)
		return ;
	wall = (t_wpath){0};
	if (!does_light(tile->wpath, ray, &wall))
		return ;
	light_wall(data, ray, wall, light);
}
