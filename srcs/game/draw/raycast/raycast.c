/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:00:35 by hle-hena          #+#    #+#             */
/*   Updated: 2025/05/25 17:59:53 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline void	hit_light(t_data *data, t_ray *ray, t_hit *hit, t_wpath wall)
{
	t_point		light_point;
	t_tlight	*tlight;
	t_list		*temp;

	light_point.x = hit->hit[ray->bounce].x * LMAP_PRECISION + (ray->dir.x > 0 ? 0.001 : -0.001 * !ray->side);
	light_point.y = hit->hit[ray->bounce].y * LMAP_PRECISION + (ray->dir.y > 0 ? 0.001 : -0.001 * ray->side);
	tlight = (data->lmap.lmap + light_point.x + light_point.y * data->lmap.wid);
	temp = ft_lstchr(tlight->flight, &wall.normal, is_correct_flight);
	if (temp)
		hit->light = (t_flight *)temp->content;
	else
		hit->light = data->temp;
}

void	handle_hit(t_data *data, t_ray *ray, t_hit *hit)
{
	t_tile	*tile;

	tile = get_tile_dict()[*(data->map->matrix + ray->curr.x +
		ray->curr.y * data->map->wid)];
	if (!tile)
		return ;
	if (!tile->is_wall)
		return ;
	if (!does_hit(tile->wpath, ray, &hit->wall[ray->bounce], &hit->pos))
		return ;
	hit->hit[ray->bounce].x = ray->origin.x + ray->dir.x * ray->precise_dist;
	hit->hit[ray->bounce].y = ray->origin.y + ray->dir.y * ray->precise_dist;
	hit_light(data, ray, hit, hit->wall[ray->bounce]);
	hit->ray_dir[ray->bounce] = ray->dir;
	hit->dist[ray->bounce] = ray->precise_dist;
	hit->texture = tile->tex_ea.img;
	hit->bounces = ray->bounce;
	if (ray->bounce != 0)
		hit->dist[ray->bounce] += hit->dist[ray->bounce - 1];
	if (hit->wall[ray->bounce].texture.reflectance && ray->bounce < MAX_BOUNCE - 1)
		handle_reflexion(hit, ray, hit->wall[ray->bounce]);
	else
		ray->running = 0;
}

t_hit	raycast(t_data *data, t_vec dir, t_vec origin)
{
	t_ray	ray;
	t_hit	hit;

	hit = (t_hit){0};
	init_ray(&ray, dir, origin);
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
