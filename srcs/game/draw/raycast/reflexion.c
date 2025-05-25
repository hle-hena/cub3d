/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflexion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 13:42:01 by hle-hena          #+#    #+#             */
/*   Updated: 2025/05/25 14:24:37 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	calc_ray(t_ray *ray)
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
	ray->curr.x = floorf(ray->origin.x);
	ray->curr.y = floorf(ray->origin.y);
	ray->running = 1;
	ray->precise_dist = 0;
}

static inline float	cross(t_vec a, t_vec b)
{
	return (a.x * b.y - a.y * b.x);
}

static inline float	dot(t_vec a, t_vec b)
{
	return (a.x * b.x + a.y * b.y);
}

t_vec	reflect(t_vec dir, t_vec normal)
{
	float	d;
	t_vec	reflected;

	d = dot(dir, normal);
	reflected.x = dir.x - 2.0f * d * normal.x;
	reflected.y = dir.y - 2.0f * d * normal.y;
	return (reflected);
}

void	handle_reflexion(t_hit *hit, t_ray *ray, t_wpath wall)
{
	ray->origin = hit->hit[ray->bounce];
	// if (ray->side == 0)
	// 	ray->origin.x += ray->dir.x > 0 ? -0.001f : 0.001f;
	// else
	// 	ray->origin.y += ray->dir.y > 0 ? -0.001f : 0.001f;
	ray->dir = reflect(ray->dir, wall.normal);
	calc_ray(ray);
	// hit->ray_dir[ray->bounce] = ray->dir;
	ray->bounce++;
}
