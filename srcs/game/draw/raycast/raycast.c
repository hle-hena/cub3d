/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:51:23 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/25 15:02:19 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline t_text	hit_text(t_data *data, t_ray *ray)
{
	if (ray->side == 0 && ray->step.x == 1)
		return (get_tile_dict()[*(data->map->matrix + ray->curr.y * data->map->wid + ray->curr.x)]->tex_ea);
	else if (ray->side == 0)
		return (get_tile_dict()[*(data->map->matrix + ray->curr.y * data->map->wid + ray->curr.x)]->tex_we);
	else if (ray->step.y == 1)
		return (get_tile_dict()[*(data->map->matrix + ray->curr.y * data->map->wid + ray->curr.x)]->tex_no);
	else
		return (get_tile_dict()[*(data->map->matrix + ray->curr.y * data->map->wid + ray->curr.x)]->tex_so);

}

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

void	handle_hit(t_data *data, t_ray *ray, t_hit *hit)
{
	t_text	texture;

	texture = hit_text(data, ray);
	if (ray->bounce == 0)
	{
		hit->m_dist = ray->precise_dist;
		hit->m_side = ray->side;
	}
	hit->hit[ray->bounce].x = ray->origin.x + ray->dir.x * ray->precise_dist;
	hit->hit[ray->bounce].y = ray->origin.y + ray->dir.y * ray->precise_dist;
	if (texture.reflectance && ray->bounce < MAX_BOUNCE)
	{
		hit->dist += ray->precise_dist;
		ray->origin = hit->hit[ray->bounce];
		if (ray->side == 0)
			ray->origin.x += ray->dir.x > 0 ? -0.001f : 0.001f;
		else
			ray->origin.y += ray->dir.y > 0 ? -0.001f : 0.001f;
		if (ray->side == 0)
			ray->dir.x = -ray->dir.x;
		else
			ray->dir.y = -ray->dir.y;
		calc_ray(ray);
		ray->bounce++;
	}
	else
	{
		hit->dist += ray->precise_dist;
		hit->side = ray->side;
		hit->texture = texture.img;
		hit->ray_hit = hit->hit[ray->bounce];
		ray->running = 0;
		hit->bounces = ray->bounce;
	}
}

void	init_ray(t_ray *ray, t_vec dir, t_player player)
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
		ray->dist.x = (player.x - floorf(player.x)) * ray->slope.x;
	else
		ray->dist.x = (ceilf(player.x) - player.x) * ray->slope.x;
	if (dir.y < 0)
		ray->dist.y = (player.y - floorf(player.y)) * ray->slope.y;
	else
		ray->dist.y = (ceilf(player.y) - player.y) * ray->slope.y;
	ray->curr.x = floorf(player.x);
	ray->curr.y = floorf(player.y);
	ray->bounce = 0;
	ray->origin.x = player.x;
	ray->origin.y = player.y;
	ray->dir = dir;
	ray->running = 1;
	ray->precise_dist = 0;
}

t_hit	raycast(t_data *data, t_vec dir, t_player player)
{
	t_ray	ray;
	t_hit	hit = (t_hit){0};

	init_ray(&ray, dir, player);
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
		if (get_tile_dict()[*(data->map->matrix + ray.curr.y * data->map->wid + ray.curr.x)]->is_wall)
			handle_hit(data, &ray, &hit);
	}
	return (hit);
}
