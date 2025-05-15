/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytrace.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 12:22:39 by hle-hena          #+#    #+#             */
/*   Updated: 2025/05/01 17:06:19 by hle-hena         ###   ########.fr       */
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

static inline int	color_attenuation(t_col col, float emittance)
{
	col.re *= emittance;
	col.gr *= emittance;
	col.bl *= emittance;
	return (calc_color(col));
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

void	handle_reflexion(t_data *data, t_lmap *map, t_trace *ray, t_light light)
{
	t_text	texture;
	t_vec	hit;

	texture = hit_text(data, ray);
	hit.x = ray->origin.x + ray->dir.x * ray->precise_dist;
	hit.y = ray->origin.y + ray->dir.y * ray->precise_dist;
	if (texture.reflectance && ray->bounce < MAX_BOUNCE - 1 && ray->emittance * pow(0.99, ray->precise_dist) > 0.01)
	{
		ray->origin = hit;
		if (ray->side == 0)
			ray->origin.x += ray->dir.x > 0 ? -0.001f : 0.001f;
		else
			ray->origin.y += ray->dir.y > 0 ? -0.001f : 0.001f;
		if (ray->origin.x < 0.001f)
			ray->origin.x = 0.001f;
		if (ray->origin.y < 0.001f)
			ray->origin.y = 0.001f;
		if (ray->origin.x > data->map->wid - 0.001f)
			ray->origin.x = data->map->wid - 0.001f;
		if (ray->origin.y > data->map->len - 0.001f)
			ray->origin.y = data->map->len - 0.001f;
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
		if (ray->side == 0)
		{
			(map->lmap + ray->curr.x + ray->curr.y * data->lmap.wid)->no_so.color = color_attenuation(light.color, pow(ATT_COEF, (ray->precise_dist * 64) / LMAP_PRECISION));
			(map->lmap + ray->curr.x + ray->curr.y * data->lmap.wid)->no_so.emittance = ray->emittance * pow(ATT_COEF, (ray->precise_dist * 64) / LMAP_PRECISION);
		}
		else
		{
			(map->lmap + ray->curr.x + ray->curr.y * data->lmap.wid)->we_ea.color = color_attenuation(light.color, pow(ATT_COEF, (ray->precise_dist * 64) / LMAP_PRECISION));
			(map->lmap + ray->curr.x + ray->curr.y * data->lmap.wid)->we_ea.emittance = ray->emittance * pow(ATT_COEF, (ray->precise_dist * 64) / LMAP_PRECISION);
		}
	}
	ray->emittance -= (1 - texture.reflectance);
}

void	init_trace(t_trace *ray, t_vec dir, t_vec origin, float emittance)
{
	origin.x *= LMAP_PRECISION;
	origin.y *= LMAP_PRECISION;
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
	ray->curr.x = floorf(origin.x);
	ray->curr.y = floorf(origin.y);
	ray->bounce = 0;
	ray->origin = origin;
	ray->dir = dir;
	ray->running = 1;
	ray->precise_dist = 0;
	ray->emittance = emittance;
}

void	handle_attenuation(t_data *data, t_lmap *lmap, t_trace *ray, t_light light)
{
	float	new;

	new = ray->emittance * pow(ATT_COEF, (ray->precise_dist * 64) / LMAP_PRECISION);
	if (new < (lmap->lmap + ray->curr.x + ray->curr.y * data->map->wid * LMAP_PRECISION)->ce_fl.emittance)
		return ;
	(lmap->lmap + ray->curr.x + ray->curr.y * data->map->wid * LMAP_PRECISION)->ce_fl.color = color_attenuation(light.color, pow(ATT_COEF, (ray->precise_dist * 64) / LMAP_PRECISION));
	(lmap->lmap + ray->curr.x + ray->curr.y * data->map->wid * LMAP_PRECISION)->ce_fl.emittance = new;
}

void	raytrace(t_data *data, t_light light, t_vec dir)
{
	t_trace	ray;

	init_trace(&ray, dir, light.pos, light.emittance);
	while (ray.running)
	{
		handle_attenuation(data, &data->lmap, &ray, light);
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
			handle_reflexion(data, &data->lmap, &ray, light);
	}
}
