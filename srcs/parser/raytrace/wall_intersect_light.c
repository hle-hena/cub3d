/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_intersect_light.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:10:35 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/07 16:43:08 by hle-hena         ###   ########.fr       */
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

static inline t_wpath	line(t_point curr, t_wpath base)
{
	return ((t_wpath){(t_vec){(base.start.x + curr.x) * LMAP_PRECISION,
		(base.start.y + curr.y) * LMAP_PRECISION}, (t_vec){(base.end.x + curr.x)
				* LMAP_PRECISION, (base.end.y + curr.y) * LMAP_PRECISION},
		(t_vec){(base.center.x + curr.x) * LMAP_PRECISION, (base.center.y
		+ curr.y) * LMAP_PRECISION}, (t_text){0}, (t_vec){0}, 0, 0, 0, 1,
			base.mode});
}

static inline float	angle(t_vec ray_dir, t_vec normal)
{
	float	dot_val;

	dot_val = ray_dir.x * normal.x + ray_dir.y * normal.y;
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
	float	closest;
	t_inter	inter;

	closest = -1;
	while (wpath)
	{
		inter = light_intersect(ray->origin, ray->dir,
				line(ray->real, *(t_wpath *)wpath->content));
		if (inter.pos > 0 - FLT_EPSILON
			&& (inter.dist < closest || closest < 0))
		{
			*wall = *(t_wpath *)wpath->content;
			if (wall->mode == 1)
				wall->normal = inter.normal;
			closest = inter.dist;
			ray->angle_factor = angle(ray->dir, wall->normal);
		}
		wpath = wpath->next;
	}
	return (is_valid_hit(ray, closest));
}
