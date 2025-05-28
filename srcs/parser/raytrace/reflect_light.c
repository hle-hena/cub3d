/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflect_light.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 10:32:59 by hle-hena          #+#    #+#             */
/*   Updated: 2025/05/28 18:05:49 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline void	calc_trace(t_trace *ray)
{
	ray->step.x = 1;
	if (ray->dir.x < 0)
		ray->step.x = -1;
	ray->step.y = 1;
	if (ray->dir.y < 0)
		ray->step.y = -1;
	ray->slope.x = HUGE_VALF;
	if (ray->dir.x != 0)
		ray->slope.x = fabs(1.0f / ray->dir.x);
	ray->slope.y = HUGE_VALF;
	if (ray->dir.y != 0)
		ray->slope.y = fabs(1.0f / ray->dir.y);
	if (ray->dir.x < 0)
		ray->dist.x = (ray->origin.x - floorf(ray->origin.x)) * ray->slope.x;
	else
		ray->dist.x = (ceilf(ray->origin.x) - ray->origin.x) * ray->slope.x;
	if (ray->dir.y < 0)
		ray->dist.y = (ray->origin.y - floorf(ray->origin.y)) * ray->slope.y;
	else
		ray->dist.y = (ceilf(ray->origin.y) - ray->origin.y) * ray->slope.y;
	ray->curr = (t_point){floorf(ray->origin.x), floorf(ray->origin.y)};
	ray->running = 1;
	ray->precise_dist = 0;
}

static inline t_vec	reflect(t_vec dir, t_vec normal)
{
	float	d;
	t_vec	reflected;

	d = dir.x * normal.x + dir.y * normal.y;
	reflected.x = dir.x - 2.0f * d * normal.x;
	reflected.y = dir.y - 2.0f * d * normal.y;
	return (reflected);
}

void	reflect_light(t_data *data, t_trace *ray, t_wpath wall, t_light light)
{
	ray->emittance *= ray->angle_factor;
	ray->last_dist = ray->precise_dist;
	ray->dir = reflect(ray->dir, wall.normal);
	ray->origin.x += ray->dir.x * 0.0001;
	ray->origin.y += ray->dir.y * 0.0001;
	ray->bounce++;
	calc_trace(ray);
	handle_light(data, ray, light);
}
