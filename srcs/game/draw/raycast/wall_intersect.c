/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_intersect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 13:37:04 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/05 15:54:54 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline t_inter	intersect_sarc(t_vec origin, t_vec dir, t_wpath arc)
{
	t_vec	delta;
	float	a;
	float	b;
	float	discriminant;
	float	sqrt_d;

	delta = vec_sub(origin, arc.center);
	a = vec_dot(dir, dir);
	b = 2.0f * vec_dot(dir, delta);
	discriminant = (b * b) - 4.0f * a * (vec_dot(delta, delta)
			- vec_len2(vec_sub(arc.start, arc.center)));
	if (discriminant < 0.0f)
		return ((t_inter){(t_vec){0}, (t_vec){0}, -1, -1});
	sqrt_d = sqrtf(discriminant);
	return (check_solutions_sarc((float [2]){(-b - sqrt_d) / (2.0f * a),
			(-b + sqrt_d) / (2.0f * a)}, origin, dir, arc));
}

static inline t_inter	intersect_sseg(t_vec origin, t_vec dir, t_wpath seg)
{
	t_vec	seg_dir;
	t_vec	delta;
	float	det;
	float	t;
	float	u;

	seg_dir = (t_vec){seg.end.x - seg.start.x, seg.end.y - seg.start.y};
	det = -dir.x * seg_dir.y + dir.y * seg_dir.x;
	if (fabs(det) < FLT_EPSILON)
		return ((t_inter){(t_vec){0}, (t_vec){0}, -1, -1});
	delta = (t_vec){seg.start.x - origin.x, seg.start.y - origin.y};
	t = (delta.x * -seg_dir.y + delta.y * seg_dir.x) / det;
	u = (-dir.y * delta.x + dir.x * delta.y) / det;
	if (t >= 0 && u >= 0 && u <= 1)
		return ((t_inter){(t_vec){0}, (t_vec){-seg_dir.y, seg_dir.x}, u, t});
	return ((t_inter){(t_vec){0}, (t_vec){0}, -1, -1});
}

t_inter	distance_intersect(t_vec origin, t_vec dir, t_wpath path_to_inter)
{
	if (path_to_inter.mode == 1)
		return (intersect_sarc(origin, dir, path_to_inter));
	return (intersect_sseg(origin, dir, path_to_inter));
}

static inline t_wpath	line(t_point curr, t_wpath base)
{
	return ((t_wpath){(t_vec){base.start.x + curr.x, base.start.y + curr.y},
		(t_vec){base.end.x + curr.x, base.end.y + curr.y}, (t_vec){base.center.x
			+ curr.x, base.center.y + curr.y}, (t_text){0}, (t_vec){0}, 0, 0, 0,
			1, base.mode});
}

//For more fun, you can try to play with the wall normal.
int	does_hit(t_list	*wpath, t_ray *ray, t_wpath *wall)
{
	t_inter	inter;
	float	closest_dist;

	closest_dist = -1;
	while (wpath)
	{
		inter = distance_intersect(ray->origin, ray->dir,
				line(ray->curr, *(t_wpath *)wpath->content));
		if (inter.pos > 0 - FLT_EPSILON
			&& (inter.dist < closest_dist || closest_dist == -1))
		{
			*wall = *(t_wpath *)wpath->content;
			closest_dist = inter.dist;
			wall->pos = inter.pos * (wall->pos_end - wall->pos_start)
				+ wall->pos_start;
			if (wall->mode == 1)
				wall->normal = inter.normal;
		}
		wpath = wpath->next;
	}
	if (closest_dist < 0)
		return (0);
	ray->precise_dist = closest_dist;
	return (1);
}
