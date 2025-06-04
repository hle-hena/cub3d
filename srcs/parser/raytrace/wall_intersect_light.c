/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_intersect_light.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:10:35 by hle-hena          #+#    #+#             */
/*   Updated: 2025/06/04 22:14:54 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
static inline t_vec	get_wall_direction(t_vec wall_start, t_vec wall_end,
	t_vec ray_dir)
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
*/

static inline float	intersect_segment(t_vec origin, t_vec dir, t_wpath d)
{
	t_vec	seg;
	t_vec	delta;
	float	det;
	float	t;
	float	u;

	seg = (t_vec){d.end.x - d.start.x, d.end.y - d.start.y};
	det = -dir.x * seg.y + dir.y * seg.x;
	if (fabs(det) < 1e-6f)
		return (-1);
	delta = (t_vec){d.start.x - origin.x, d.start.y - origin.y};
	t = (delta.x * -seg.y + delta.y * seg.x) / det;
	u = (-dir.y * delta.x + dir.x * delta.y) / det;
	if (t >= 0 && u >= 0 && u <= 1)
		return (t);
	return (-1);
}

static inline t_wpath	line(t_point curr, t_wpath base)
{
	return ((t_wpath){(t_vec){(base.start.x + curr.x) * LMAP_PRECISION,
		(base.start.y + curr.y) * LMAP_PRECISION}, (t_vec){(base.end.x + curr.x)
				* LMAP_PRECISION, (base.end.y + curr.y) * LMAP_PRECISION},
		(t_text){0}, (t_vec){0}, 0});
}

static inline float	angle(t_vec ray_dir, t_wpath seg)
{
	t_vec	wall_dir;
	t_vec	wall_normal;
	float	len;
	float	dot_val;

	wall_dir.x = seg.end.x - seg.start.x;
	wall_dir.y = seg.end.y - seg.start.y;
	len = sqrtf(wall_dir.x * wall_dir.x + wall_dir.y * wall_dir.y);
	if (len == 0.0f)
		return (0.0f);
	wall_dir.x /= len;
	wall_dir.y /= len;
	wall_normal.x = -wall_dir.y;
	wall_normal.y = wall_dir.x;
	dot_val = ray_dir.x * wall_normal.x + ray_dir.y * wall_normal.y;
	dot_val = fabs(dot_val);
	if (dot_val < 0)
		dot_val = 0;
	if (dot_val > 1)
		dot_val = 1;
	return (sqrtf(sqrtf(dot_val)));
}

static inline int	is_valid_hit(t_trace *ray, float dist)
{
	t_vec	hit;

	if (dist < 0)
		return (0);
	hit.x = ray->origin.x + ray->dir.x * dist;
	hit.y = ray->origin.y + ray->dir.y * dist;
	if ((ray->dir.x > 0 && floorf(hit.x) > ray->curr.x)
		|| (ray->dir.x < 0 && floorf(hit.x) < ray->curr.x)
		|| (ray->dir.y > 0 && floorf(hit.y) > ray->curr.y)
		|| (ray->dir.y < 0 && floorf(hit.y) < ray->curr.y))
		return (0);
	ray->precise_dist = ray->last_dist + dist;
	ray->origin = hit;
	return (1);
}

int	does_light(t_list *wpath, t_trace *ray, t_wpath *wall)
{
	float	dist;
	float	temp;

	dist = -1;
	while (wpath)
	{
		temp = intersect_segment(ray->origin, ray->dir,
			line(ray->real, *(t_wpath *)wpath->content));
		if (temp < -0.5f)
		{
			wpath = wpath->next;
			continue ;
		}
		if (temp < dist || dist == -1)
		{
			*wall = *(t_wpath *)wpath->content;
			ray->angle_factor = angle(ray->dir, *wall);
			dist = temp;
		}
		wpath = wpath->next;
	}
	return (is_valid_hit(ray, dist));
}
