/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytrace.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 12:22:39 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/28 14:07:24 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline t_text	hit_text(t_data *data, t_trace *ray)
{
	if (ray->side == 0 && ray->step.x == 1)
		return (get_tile_dict()[*(data->map->matrix + ray->real.y * data->map->wid + ray->real.x)]->tex_ea);
	else if (ray->side == 0)
		return (get_tile_dict()[*(data->map->matrix + ray->real.y * data->map->wid + ray->real.x)]->tex_we);
	else if (ray->step.y == 1)
		return (get_tile_dict()[*(data->map->matrix + ray->real.y * data->map->wid + ray->real.x)]->tex_no);
	else
		return (get_tile_dict()[*(data->map->matrix + ray->real.y * data->map->wid + ray->real.x)]->tex_so);

}

void	calc_trace(t_trace *ray)
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

void	handle_reflexion(t_data *data, t_lmap *map, t_trace *ray)
{
	t_text	texture;
	t_vec	hit;

	texture = hit_text(data, ray);
	hit.x = ray->origin.x + ray->dir.x * ray->precise_dist;
	hit.y = ray->origin.y + ray->dir.y * ray->precise_dist;
	ray->origin = hit;
	if (ray->side == 0)
		ray->origin.x += ray->dir.x > 0 ? -0.001f : 0.001f;
	else
		ray->origin.y += ray->dir.y > 0 ? -0.001f : 0.001f;
	if (texture.reflectance && ray->bounce < MAX_BOUNCE - 1 && ray->emittance > 0.01)
	{
		if (ray->side == 0)
			ray->dir.x = -ray->dir.x;
		else
			ray->dir.y = -ray->dir.y;
		calc_trace(ray);
		ray->bounce++;
	}
	else
	{
		ray->running = 0;
		*(map->lmap + ray->curr.x + ray->curr.y * data->map->wid * LMAP_PRECISION) = ray->emittance * pow(0.99, ray->precise_dist);
	}
	ray->emittance -= (1 - texture.reflectance);
}

void	init_trace(t_trace *ray, t_vec dir, t_vec origin, float emittance)
{
	ray->step.x = 1;
	if (dir.x < 0)
		ray->step.x = -1;
	ray->step.y = 1;
	if (dir.y < 0)
		ray->step.y = -1;
	ray->slope.x = HUGE_VALF;
	if (dir.x != 0)
		ray->slope.x = fabs(1.0f / dir.x);
	ray->slope.y = HUGE_VALF;
	if (dir.y != 0)
		ray->slope.y = fabs(1.0f / dir.y);
	if (dir.x < 0)
		ray->dist.x = (origin.x - floorf(origin.x)) * ray->slope.x;
	else
		ray->dist.x = (ceilf(origin.x) - origin.x) * ray->slope.x;
	if (dir.y < 0)
		ray->dist.y = (origin.y - floorf(origin.y)) * ray->slope.y;
	else
		ray->dist.y = (ceilf(origin.y) - origin.y) * ray->slope.y;
	ray->curr.x = floorf(origin.x * LMAP_PRECISION);
	ray->curr.y = floorf(origin.y * LMAP_PRECISION);
	ray->bounce = 0;
	ray->origin = origin;
	ray->dir = dir;
	ray->running = 1;
	ray->precise_dist = 0;
	ray->emittance = emittance;
}

void	handle_attenuation(t_data *data, t_lmap *lmap, t_trace *ray)
{
	*(lmap->lmap + ray->curr.x + ray->curr.y * data->map->wid * LMAP_PRECISION) = ray->emittance * pow(0.99, ray->precise_dist);
}

void	raytrace(t_data *data, t_vec origin, t_vec dir, float emittance)
{
	t_trace	ray;

	init_trace(&ray, dir, origin, emittance);
	while (ray.running)
	{
		handle_attenuation(data, &data->lmap, &ray);
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
		ray.real.x = ray.curr.x / LMAP_PRECISION;
		ray.real.y = ray.curr.y / LMAP_PRECISION;
		if (get_tile_dict()[*(data->map->matrix + ray.real.y * data->map->wid + ray.real.x)]->is_wall)
			handle_reflexion(data, &data->lmap, &ray);
	}
}
