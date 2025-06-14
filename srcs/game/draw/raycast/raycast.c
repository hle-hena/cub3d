/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:00:35 by hle-hena          #+#    #+#             */
/*   Updated: 2025/06/06 11:05:23 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline void	hit_light(t_data *data, t_ray *ray, t_hit *hit, t_wpath wall)
{
	t_point		light_point;
	t_tlight	*tlight;
	t_list		*temp;

	light_point.x = hit->hit[ray->bounce].x * LMAP_PRECISION;
	light_point.y = hit->hit[ray->bounce].y * LMAP_PRECISION;
	light_point.x = ft_max(ft_min(light_point.x, data->lmap.wid - 1), 0);
	light_point.y = ft_max(ft_min(light_point.y, data->lmap.len - 1), 0);
	tlight = (data->lmap.lmap + light_point.x + light_point.y * data->lmap.wid);
	temp = ft_lstchr(tlight->flight, &wall.normal, is_correct_flight);
	if (temp)
		hit->light[ray->bounce] = (t_flight *)temp->content;
	else
		hit->light[ray->bounce] = data->empty->flight;
}

static inline int	is_outside(t_data *data, t_ray *ray, t_hit *hit)
{
	int	is_out;
	int	should_stop;

	is_out = 0;
	should_stop = 0;
	if (ray->curr.x < 0)
	{
		is_out = 1;
		if (ray->dir.x <= 0)
			should_stop = 1;
	}
	if (ray->curr.y < 0)
	{
		is_out = 1;
		if (ray->dir.y <= 0)
			should_stop = 1;
	}
	if (ray->curr.x >= data->map->wid)
	{
		is_out = 1;
		if (ray->dir.x >= 0)
			should_stop = 1;
	}
	if (ray->curr.y >= data->map->len)
	{
		is_out = 1;
		if (ray->dir.y >= 0)
			should_stop = 1;
	}
	if (should_stop)
	{
		hit->dist[ray->bounce] = ray->precise_dist;
		hit->texture[ray->bounce] = data->empty->texture.img;
		hit->light[ray->bounce] = data->empty->flight;
		ray->running = 0;
	}
	return (is_out);
}

void	handle_hit(t_data *data, t_ray *ray, t_hit *hit)
{
	t_tile	*tile;

	if (is_outside(data, ray, hit))
		return ;
	tile = get_tile_dict()[*(data->map->matrix + ray->curr.x +
		ray->curr.y * data->map->wid)];
	if (!tile)
		return ;
	if (!tile->is_wall)
		return ;
	if (!does_hit(tile->wpath, ray, &hit->wall[ray->bounce]))
		return ;
	hit->hit[ray->bounce].x = ray->origin.x + ray->dir.x * ray->precise_dist;
	hit->hit[ray->bounce].y = ray->origin.y + ray->dir.y * ray->precise_dist;
	hit_light(data, ray, hit, hit->wall[ray->bounce]);
	hit->ray_dir[ray->bounce] = ray->dir;
	hit->texture[ray->bounce] = tile->tex_ea.img;
	hit->bounces = ray->bounce;
	hit->dist[ray->bounce] = ray->precise_dist;
	if (ray->bounce != 0)
		hit->dist[ray->bounce] += hit->dist[ray->bounce - 1];
	if (hit->wall[ray->bounce].reflectance && ray->bounce < MAX_BOUNCE - 1)
		handle_reflexion(data, hit, ray, hit->wall[ray->bounce]);
	else
		ray->running = 0;
}

t_hit	raycast(t_data *data, t_vec dir, t_vec origin)
{
	t_ray	ray;
	t_hit	hit;

	hit = (t_hit){0};
	init_ray(&ray, dir, origin);
	handle_hit(data, &ray, &hit);
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
		handle_hit(data, &ray, &hit);
	}
	return (hit);
}
