/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:51:23 by hle-hena          #+#    #+#             */
/*   Updated: 2025/05/20 11:36:24 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline t_vec	get_wall_direction(t_vec wall_start, t_vec wall_end, t_vec ray_dir)
{
	float dx = wall_end.x - wall_start.x;
	float dy = wall_end.y - wall_start.y;
	float nx = -dy;
	float ny = dx;
	float dot = nx * ray_dir.x + ny * ray_dir.y;

	if (dot > 0)
	{
		nx = -nx;
		ny = -ny;
	}
	return ((t_vec){nx, ny});
}

static inline void	hit_light(t_data *data, t_ray *ray, t_hit *hit, t_wpath wall)
{
	t_vec		normal;
	t_point		light_point;
	t_tlight	*tlight;
	t_list		*temp;

	normal = get_wall_direction(wall.start, wall.end, ray->dir);
	light_point.x = hit->hit[ray->bounce].x * LMAP_PRECISION + (ray->dir.x > 0 ? 0.001 : -0.001 * !ray->side);
	light_point.y = hit->hit[ray->bounce].y * LMAP_PRECISION + (ray->dir.y > 0 ? 0.001 : -0.001 * ray->side);
	tlight = (data->lmap.lmap + light_point.x + light_point.y * data->lmap.wid);
	temp = ft_lstchr(tlight->flight, &normal, is_correct_flight);
	// temp = tlight->flight;
	// (void)normal;
	if (temp)
		hit->light = (t_flight *)temp->content;
	else
		hit->light = data->temp;
}

static inline t_tile	*hit_tile(t_data *data, t_ray *ray)
{
	return (get_tile_dict()[*(data->map->matrix + ray->curr.y * data->map->wid + ray->curr.x)]);
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

static inline float	cross(t_vec a, t_vec b)
{
	return (a.x * b.y - a.y * b.x);
}

static inline float	intersect_segment(t_vec origin, t_vec dir, t_wpath d)
{
	t_vec seg = {d.end.x - d.start.x, d.end.y - d.start.y};

	float det = -dir.x * seg.y + dir.y * seg.x;
	if (fabs(det) < 1e-6f)
		return -1;

	t_vec delta = {d.start.x - origin.x, d.start.y - origin.y};
	float t = (delta.x * -seg.y + delta.y * seg.x) / det;
	float u = (-dir.y * delta.x + dir.x * delta.y) / det;
	if (t >= 0 && u >= 0 && u <= 1)
		return t;
	return -1;
}

static inline t_wpath	line(t_point curr, t_wpath base)
{
	return ((t_wpath){(t_vec){base.start.x + curr.x, base.start.y + curr.y},
		(t_vec){base.end.x + curr.x, base.end.y + curr.y}, (t_text){0}});
}

static inline int	does_hit(t_list	*wpath, t_ray *ray, t_wpath *wall)
{
	float	dist;
	float	temp;

	dist = -1;
	while (wpath)
	{
		temp = intersect_segment(ray->origin, ray->dir,
			line(ray->curr, *(t_wpath *)wpath->content));
		if (temp < -0.5f)
		{
			wpath = wpath->next;
			continue ;
		}
		if (temp < dist || dist == -1)
		{
			*wall = *(t_wpath *)wpath->content;
			dist = temp;
		}
		wpath = wpath->next;
	}
	if (dist < 0)
		return (0);
	ray->precise_dist = dist;
	return (1);
}

void	handle_hit(t_data *data, t_ray *ray, t_hit *hit)
{
	t_text	texture;
	t_tile	*tile;
	t_wpath	wall;

	tile = hit_tile(data, ray);
	wall = (t_wpath){0};
	if (!does_hit(tile->wpath, ray, &wall))
		return ;
	hit->hit[ray->bounce].x = ray->origin.x + ray->dir.x * ray->precise_dist;
	hit->hit[ray->bounce].y = ray->origin.y + ray->dir.y * ray->precise_dist;
	texture = wall.texture;
	hit_light(data, ray, hit, wall);
	if (texture.reflectance && ray->bounce < MAX_BOUNCE - 1)
	{
		hit->dist[ray->bounce] = ray->precise_dist;
		if (ray->bounce != 0)
			hit->dist[ray->bounce] += hit->dist[ray->bounce - 1];
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
		hit->side[ray->bounce] = ray->side;
		hit->ray_dir[ray->bounce] = ray->dir;
		ray->bounce++;
		if (get_tile_dict()[*(data->map->matrix + ray->curr.y * data->map->wid + ray->curr.x)]->is_wall)
		{
			ray->side = !ray->side;
			handle_hit(data, ray, hit);
		}
	}
	else
	{
		hit->dist[ray->bounce] = ray->precise_dist;
		if (ray->bounce != 0)
			hit->dist[ray->bounce] += hit->dist[ray->bounce - 1];
		hit->side[ray->bounce] = ray->side;
		hit->texture = texture.img;
		ray->running = 0;
		hit->bounces = ray->bounce;
		hit->ray_dir[ray->bounce] = ray->dir;
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
