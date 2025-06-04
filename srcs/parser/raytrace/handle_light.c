/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_light.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:32:35 by hle-hena          #+#    #+#             */
/*   Updated: 2025/06/04 22:28:05 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline void	light_adjacent(t_flight *flight, float emittance,
	t_light light)
{
	if (emittance > flight->emittance)
	{
		flight->color = calc_color((t_col){light.color.re * emittance,
					light.color.gr * emittance, light.color.bl * emittance});
		flight->emittance = emittance;
	}
}

static inline void	light_wall(t_data *data, t_trace *ray, t_wpath wall,
	t_light light)
{
	t_flight	*flight;
	t_vec		offset;
	float		emittance;

	ray->angle_factor = ray->angle_factor - pow(2.71828, -ray->angle_factor
		* 0.1 * (ray->precise_dist / LMAP_PRECISION)) + 1;
	if (ray->angle_factor > 1)
		ray->angle_factor = 1;
	emittance = ray->angle_factor * ray->emittance;
	ray->curr = (t_point){(int)ray->origin.x, (int)ray->origin.y};
	flight = find_flight(data, ray->curr, wall);
	if (emittance > flight->emittance)
	{
		flight->color = calc_color((t_col){light.color.re * emittance,
					light.color.gr * emittance, light.color.bl * emittance});
		flight->emittance = emittance;
	}
	offset.x = ray->origin.x - (int)ray->origin.x;
	offset.y = ray->origin.y - (int)ray->origin.y;
	if (offset.x < 0.001)
		light_adjacent(find_flight(data, (t_point){ray->curr.x - 1, ray->curr.y},
				wall), emittance, light);
	else if (offset.x > 0.999)
		light_adjacent(find_flight(data, (t_point){ray->curr.x + 1, ray->curr.y},
				wall), emittance, light);
	if (offset.y < 0.001)
		light_adjacent(find_flight(data, (t_point){ray->curr.x, ray->curr.y - 1},
				wall), emittance, light);
	else if (offset.y > 0.999)
		light_adjacent(find_flight(data, (t_point){ray->curr.x, ray->curr.y + 1},
				wall), emittance, light);
	ray->running = 0;
	ray->emittance -= (1 - wall.reflectance);
}

static inline void	light_floor(t_data *data, t_trace *ray, t_light light)
{
	t_tlight	*tlight;

	tlight = (data->lmap.lmap + ray->curr.x + ray->curr.y * data->lmap.wid);
	ray->emittance = (ray->emittance)
		/ (1 + ATT_COEF * pow((ray->precise_dist / LMAP_PRECISION), 2));
	// if (ray->emittance < 0)
	// 	return (ray->running = 0, VOID);
	if (ray->emittance < tlight->ce_fl.emittance)
		return ;
	tlight->ce_fl.color = calc_color((t_col){light.color.re
				* ray->emittance, light.color.gr * ray->emittance,
				light.color.bl * ray->emittance});
	tlight->ce_fl.emittance = ray->emittance;
}

static inline int	is_outside(t_data *data, t_trace *ray)
{
	int	is_out;
	int	should_stop;

	is_out = 0;
	should_stop = 0;
	if (ray->curr.x < 0)
	{
		is_out = 1;
		if (ray->dir.x <= 0)
			should_stop = 1;
	}
	if (ray->curr.y < 0)
	{
		is_out = 1;
		if (ray->dir.y <= 0)
			should_stop = 1;
	}
	if (ray->curr.x >= data->lmap.wid)
	{
		is_out = 1;
		if (ray->dir.x >= 0)
			should_stop = 1;
	}
	if (ray->curr.y >= data->lmap.len)
	{
		is_out = 1;
		if (ray->dir.y >= 0)
			should_stop = 1;
	}
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
	tile = get_tile_dict()[*(data->map->matrix + ray->real.y * data->map->wid
		+ ray->real.x)];
	if (!tile)
		return ;
	ray->precise_dist += ray->last_dist;
	light_floor(data, ray, light);
	if (!tile->is_wall)
		return ;
	wall = (t_wpath){0};
	if (!does_light(tile->wpath, ray, &wall))
		return ;
	light_wall(data, ray, wall, light);
}
