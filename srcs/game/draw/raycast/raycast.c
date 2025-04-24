/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:51:23 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/24 16:54:01 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	text_info(t_data *data, t_ray ray, t_hit *hit)
{
	if (ray.side == 0 && ray.step.x == 1)
		hit->texture = get_tile_dict()[*(data->map->matrix + ray.curr.y * data->map->wid + ray.curr.x)]->tex_ea;
	else if (ray.side == 0)
		hit->texture = get_tile_dict()[*(data->map->matrix + ray.curr.y * data->map->wid + ray.curr.x)]->tex_we;
	else if (ray.step.y == 1)
		hit->texture = get_tile_dict()[*(data->map->matrix + ray.curr.y * data->map->wid + ray.curr.x)]->tex_no;
	else
		hit->texture = get_tile_dict()[*(data->map->matrix + ray.curr.y * data->map->wid + ray.curr.x)]->tex_so;
}

t_hit	bounce_info(t_data *data, t_player player, t_ray ray, t_vec dir)
{
	t_hit	hit;
	t_hit	bounce;

	if (ray.side == 0)
		hit.dist = (ray.curr.x - player.x + (1 - ray.step.x) / 2.0f) / dir.x;
	else
		hit.dist = (ray.curr.y - player.y + (1 - ray.step.y) / 2.0f) / dir.y;
	hit.ray_hit.x = player.x + dir.x * hit.dist;
	hit.ray_hit.y = player.y + dir.y * hit.dist;
	hit.side = ray.side;
	if (get_tile_dict()[*(data->map->matrix + ray.curr.y * data->map->wid + ray.curr.x)]->floor_height == 1 && ray.bounce < 64)
	{
		player.x = hit.ray_hit.x;
		player.y = hit.ray_hit.y;
		if (ray.side == 0)
			dir.x = -dir.x;
		else
			dir.y = -dir.y;
		bounce = raycast(data, dir, player, ray.bounce + 1);
		hit.texture = bounce.texture;
		hit.ray_hit.x = bounce.ray_hit.x;
		hit.ray_hit.y = bounce.ray_hit.y;
		if (bounce.dist > hit.dist)
			hit.dist = bounce.dist;
		hit.side = bounce.side;
	}
	else
		text_info(data, ray, &hit);
	hit.bounces = ray.bounce;
	return (hit);
}

void	init_ray(t_ray *ray, t_vec dir, t_player player, int bounce)
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
	ray->bounce = bounce;
}

t_hit	raycast(t_data *data, t_vec dir, t_player player, int bounce)
{
	t_ray	ray;

	init_ray(&ray, dir, player, bounce);
	while (1)
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
		if (get_tile_dict()[*(data->map->matrix + ray.curr.y * data->map->wid + ray.curr.x)]->is_wall)
			return (bounce_info(data, player, ray, dir));
	}
}
