/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_intersect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 13:37:04 by hle-hena          #+#    #+#             */
/*   Updated: 2025/05/27 14:36:47 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline float	intersect_segment(t_vec origin, t_vec dir, t_wpath d,
	float *percent)
{
	t_vec seg = {d.end.x - d.start.x, d.end.y - d.start.y};

	float det = -dir.x * seg.y + dir.y * seg.x;
	if (fabs(det) < 1e-6f)
		return -1;

	t_vec delta = {d.start.x - origin.x, d.start.y - origin.y};
	float t = (delta.x * -seg.y + delta.y * seg.x) / det;
	float u = (-dir.y * delta.x + dir.x * delta.y) / det;
	if (t >= 0 && u >= 0 && u <= 1)
		return (*percent = u, t);
	return -1;
}

static inline t_wpath	line(t_point curr, t_wpath base)
{
	return ((t_wpath){(t_vec){base.start.x + curr.x, base.start.y + curr.y},
		(t_vec){base.end.x + curr.x, base.end.y + curr.y}, (t_text){0},
		(t_vec){0}, 0});
}

int	is_invalid_hit(t_ray *ray, t_wpath *wall, float dist)
{
	static t_point	last_curr = (t_point){0};
	static t_wpath	last_wall = (t_wpath){0};

	if (dist < 0)
		return (1);
	if (ray->start == 1)
	{
		last_curr = ray->curr;
		last_wall = *wall;
		ray->start = 0;
		return (0);
	}
	if (last_curr.x == ray->curr.x && last_curr.y == ray->curr.y
		&& last_wall.start.x == wall->start.x
		&& last_wall.start.y == wall->start.y
		&& last_wall.end.x == wall->end.x && last_wall.end.y == wall->end.y)
		return (1);
	last_curr = ray->curr;
	last_wall = *wall;
	return (0);
}

int	does_hit(t_list	*wpath, t_ray *ray, t_wpath *wall, float *hit_percent)
{
	float	dist;
	float	temp;
	float	percent;

	dist = -1;
	while (wpath)
	{
		temp = intersect_segment(ray->origin, ray->dir,
			line(ray->curr, *(t_wpath *)wpath->content), &percent);
		if (temp < 0)
		{
			wpath = wpath->next;
			continue ;
		}
		if (temp < dist || dist == -1)
		{
			*wall = *(t_wpath *)wpath->content;
			dist = temp;
			*hit_percent = percent;
		}
		wpath = wpath->next;
	}
	// if (is_invalid_hit(ray, wall, dist))
	// if (dist < 0.001f)
	if (dist < 0)
		return (0);
	ray->precise_dist = dist;
	return (1);
}
