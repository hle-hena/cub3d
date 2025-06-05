/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytrace.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 10:11:09 by hle-hena          #+#    #+#             */
/*   Updated: 2025/06/05 11:25:06 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline void	light_floor(t_data *data, t_trace *ray, t_light light)
{
	float	emittance;

	if (ray->curr.x < 0 || ray->curr.x >= data->lmap.wid || ray->curr.y < 0
		|| ray->curr.y >= data->lmap.len)
		return ;
	emittance = ray->emittance;
	if (emittance < (data->lmap.lmap + ray->curr.x + ray->curr.y
			* data->map->wid * LMAP_PRECISION)->ce_fl.emittance)
		return ;
	(data->lmap.lmap + ray->curr.x + ray->curr.y * data->map->wid
		* LMAP_PRECISION)->ce_fl.color = calc_color((t_col){light.color.re
					* emittance, light.color.gr * emittance,
					light.color.bl * emittance});
	(data->lmap.lmap + ray->curr.x + ray->curr.y * data->map->wid
		* LMAP_PRECISION)->ce_fl.emittance = emittance;
}

void	raytrace(t_data *data, t_light light, t_vec dir)
{
	t_trace	ray;

	init_trace(&ray, dir, light.pos, light.emittance);
	light_floor(data, &ray, light);
	handle_light(data, &ray, light);
	while (ray.running)
	{
		if (ray.dist.x < ray.dist.y)
		{
			ray.precise_dist = ray.dist.x;
			ray.curr.x += ray.step.x;
			ray.dist.x += ray.slope.x;
			ray.side = 0;
		}
		else
		{
			ray.precise_dist = ray.dist.y;
			ray.curr.y += ray.step.y;
			ray.dist.y += ray.slope.y;
			ray.side = 1;
		}
		handle_light(data, &ray, light);
	}
}
