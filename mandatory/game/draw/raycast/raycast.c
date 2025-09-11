/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:51:23 by hle-hena          #+#    #+#             */
/*   Updated: 2025/09/01 15:16:37 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_hit	hit_info(t_data *data, t_ray ray, t_vec dir)
{
	t_hit	h;

	if (ray.side == 0)
		h.dist = (ray.curr.x - ray.ori.x + (1 - ray.step.x) / 2.0f) / dir.x;
	else
		h.dist = (ray.curr.y - ray.ori.y + (1 - ray.step.y) / 2.0f) / dir.y;
	h.ray_hit = (t_vec){ray.ori.x + dir.x * h.dist, ray.ori.y + dir.y * h.dist};
	if (ray.step.x == -1)
		h.ray_hit.y = (int)h.ray_hit.y + 1.0f - h.ray_hit.y + (int)h.ray_hit.y;
	if (ray.step.y == 1)
		h.ray_hit.x = (int)h.ray_hit.x + 1.0f - h.ray_hit.x + (int)h.ray_hit.x;
	if (ray.side == 0 && ray.step.x == 1)
		h.texture = get_tile_dict()[*(data->map->matrix + ray.curr.y
				* data->map->wid + ray.curr.x)]->tex_ea;
	else if (ray.side == 0)
		h.texture = get_tile_dict()[*(data->map->matrix + ray.curr.y
				* data->map->wid + ray.curr.x)]->tex_we;
	else if (ray.step.y == 1)
		h.texture = get_tile_dict()[*(data->map->matrix + ray.curr.y
				* data->map->wid + ray.curr.x)]->tex_no;
	else
		h.texture = get_tile_dict()[*(data->map->matrix + ray.curr.y
				* data->map->wid + ray.curr.x)]->tex_so;
	h.side = ray.side;
	return (h);
}

void	init_ray(t_ray *ray, t_vec dir, t_player player)
{
	ray->step.x = 1 - (dir.x < 0) * 2;
	ray->step.y = 1 - (dir.y < 0) * 2;
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
	ray->curr.x = (int)floorf(player.x);
	ray->curr.y = (int)floorf(player.y);
	ray->running = 1;
	ray->ori = (t_vec){player.x, player.y};
}

void	handle_hit(t_data *data, t_ray *ray, t_hit *hit, t_vec dir)
{
	t_tile	*tile;

	tile = get_tile_dict()[*(data->map->matrix + ray->curr.x
			+ ray->curr.y * data->map->wid)];
	if (!tile)
		return ;
	if (tile->is_wall)
	{
		*hit = hit_info(data, *ray, dir);
		ray->running = 0;
	}
}

t_hit	raycast(t_data *data, t_vec dir, t_player player)
{
	t_ray	ray;
	t_hit	hit;

	hit = (t_hit){0};
	init_ray(&ray, dir, player);
	handle_hit(data, &ray, &hit, dir);
	while (ray.running)
	{
		if (ray.dist.x < ray.dist.y)
		{
			ray.dist.x += ray.slope.x;
			ray.curr.x += ray.step.x;
			ray.side = 0;
		}
		else
		{
			ray.dist.y += ray.slope.y;
			ray.curr.y += ray.step.y;
			ray.side = 1;
		}
		handle_hit(data, &ray, &hit, dir);
	}
	return (hit);
}
