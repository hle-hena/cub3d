/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:51:23 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/25 14:13:23 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline void	text_info(t_data *data, t_ray ray, t_hit *hit, t_text *text)
{
	t_text	temp;

	if (ray.side == 0 && ray.step.x == 1)
		temp = get_tile_dict()[*(data->map->matrix + ray.curr.y * data->map->wid + ray.curr.x)]->tex_ea;
	else if (ray.side == 0)
		temp = get_tile_dict()[*(data->map->matrix + ray.curr.y * data->map->wid + ray.curr.x)]->tex_we;
	else if (ray.step.y == 1)
		temp = get_tile_dict()[*(data->map->matrix + ray.curr.y * data->map->wid + ray.curr.x)]->tex_no;
	else
		temp = get_tile_dict()[*(data->map->matrix + ray.curr.y * data->map->wid + ray.curr.x)]->tex_so;
	*text = temp;
	hit->texture = temp.img;
}

t_hit	bounce_info(t_data *data, t_player player, t_ray ray, t_vec dir, float precise_dist)
{
	t_hit	hit;
	t_hit	bounce;
	t_text	temp;

	hit.r_dist = precise_dist;
	hit.ray_hit.x = player.x + dir.x * hit.r_dist;
	hit.ray_hit.y = player.y + dir.y * hit.r_dist;
	hit.m_hit = hit.ray_hit;
	hit.side = ray.side;
	hit.m_dist = hit.r_dist;
	hit.m_side = ray.side;
	text_info(data, ray, &hit, &temp);
	if (temp.reflectance && ray.bounce < MAX_BOUNCE)
	{
		player.x = hit.ray_hit.x;
		player.y = hit.ray_hit.y;
		const float epsilon = 0.001f;
		if (ray.side == 0)
			player.x += dir.x > 0 ? -epsilon : epsilon;
		else
			player.y += dir.y > 0 ? -epsilon : epsilon;
		if (ray.side == 0)
			dir.x = -dir.x;
		else
			dir.y = -dir.y;
		bounce = raycast(data, dir, player, ray.bounce + 1);
		hit.texture = bounce.texture;
		hit.ray_hit = bounce.ray_hit;
		hit.m_dist += bounce.m_dist;
		hit.side = bounce.side;
	}
	hit.bounces = ray.bounce;
	return hit;
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
	float	precise_dist = 0.0f;

	init_ray(&ray, dir, player, bounce);
	while (1)
	{
		if (ray.dist.x < ray.dist.y)
		{
			precise_dist = ray.dist.x;
			ray.curr.x += ray.step.x;
			ray.dist.x += ray.slope.x;
			ray.side = 0;
		}
		else
		{
			precise_dist = ray.dist.y;
			ray.curr.y += ray.step.y;
			ray.dist.y += ray.slope.y;
			ray.side = 1;
		}
		if (get_tile_dict()[*(data->map->matrix + ray.curr.y * data->map->wid + ray.curr.x)]->is_wall)
			return (bounce_info(data, player, ray, dir, precise_dist));
	}
}
