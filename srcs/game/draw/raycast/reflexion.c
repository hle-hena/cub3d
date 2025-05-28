/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflexion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 13:42:01 by hle-hena          #+#    #+#             */
/*   Updated: 2025/05/28 10:39:11 by hle-hena         ###   ########.fr       */
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

static inline float	dot(t_vec a, t_vec b)
{
	return (a.x * b.x + a.y * b.y);
}

static inline t_vec	reflect(t_vec dir, t_vec normal)
{
	float	d;
	t_vec	reflected;

	d = dot(dir, normal);
	reflected.x = dir.x - 2.0f * d * normal.x;
	reflected.y = dir.y - 2.0f * d * normal.y;
	return (reflected);
}

t_vec	normalize(t_vec v)
{
	//I don't think that func is supposed to be here x)
	float	len = sqrtf(v.x * v.x + v.y * v.y);
	if (len == 0.0f)
		return (t_vec){0, 0};
	return (t_vec){v.x / len, v.y / len};
}

void	handle_reflexion(t_data *data, t_hit *hit, t_ray *ray, t_wpath wall)
{
	ray->dir = reflect(ray->dir, wall.normal);
	ray->origin = hit->hit[ray->bounce];
	ray->origin.x += ray->dir.x * 0.0001;
	ray->origin.y += ray->dir.y * 0.0001;
	ray->bounce++;
	calc_ray(ray);
	handle_hit(data, ray, hit);
}
