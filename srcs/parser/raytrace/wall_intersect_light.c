/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_intersect_light.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:10:35 by hle-hena          #+#    #+#             */
/*   Updated: 2025/06/05 11:20:41 by hle-hena         ###   ########.fr       */
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

static inline t_vec vec_sub(t_vec a, t_vec b) {
	return (t_vec){a.x - b.x, a.y - b.y};
}

static inline t_vec vec_add(t_vec a, t_vec b) {
	return (t_vec){a.x + b.x, a.y + b.y};
}

static inline t_vec vec_scale(t_vec a, float s) {
	return (t_vec){a.x * s, a.y * s};
}

static inline float vec_dot(t_vec a, t_vec b) {
	return a.x * b.x + a.y * b.y;
}

static inline float vec_cross(t_vec a, t_vec b) {
	return a.x * b.y - a.y * b.x;
}

static inline float vec_len2(t_vec a) {
	return a.x * a.x + a.y * a.y;
}

static inline t_vec vec_normalize(t_vec a) {
	float len = sqrtf(vec_len2(a));
	if (len < FLT_EPSILON)
		return (t_vec){0, 0};
	return vec_scale(a, 1.0f / len);
}

static inline float	intersect_arc(t_vec origin, t_vec dir, t_wpath arc,
	t_vec *normal_out)
{
	t_vec delta = vec_sub(origin, arc.center);
	t_vec A = arc.start;
	t_vec B = arc.end;
	t_vec C = arc.center;
	float r2 = vec_len2(vec_sub(A, C));
	float a = vec_dot(dir, dir);
	float b = 2.0f * vec_dot(dir, delta);
	float c = vec_dot(delta, delta) - r2;
	float discriminant = b * b - 4.0f * a * c;
	if (discriminant < 0.0f)
		return (-1.0f);
	float sqrt_d = sqrtf(discriminant);
	float t1 = (-b - sqrt_d) / (2.0f * a);
	float t2 = (-b + sqrt_d) / (2.0f * a);
	for (int i = 0; i < 2; ++i)
	{
		float t = (i == 0) ? t1 : t2;
		if (t < 0.0f)
			continue;
		t_vec hit = vec_add(origin, vec_scale(dir, t));
		t_vec from_center = vec_sub(hit, C);
		t_vec v_hit = vec_normalize(from_center);
		if (A.x == B.x && A.y == B.y)
		{
			*normal_out = v_hit;
			return (t);
		}
		t_vec v1 = vec_normalize(vec_sub(A, C));
		t_vec v2 = vec_normalize(vec_sub(B, C));
		float cross_arc = vec_cross(v1, v2);
		float cross1 = vec_cross(v1, v_hit);
		float cross2 = vec_cross(v_hit, v2);
		int on_arc;
		if (cross_arc >= 0)
			on_arc = (cross1 >= -FLT_EPSILON && cross2 >= -FLT_EPSILON);
		else
			on_arc = (cross1 >= -FLT_EPSILON || cross2 >= -FLT_EPSILON);

		if (on_arc)
		{
			*normal_out = v_hit;
			return (t);
		}
	}
	return (-1.0f);
}

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
		(t_vec){(base.center.x + curr.x) * LMAP_PRECISION, (base.center.y
		+ curr.y) * LMAP_PRECISION}, (t_text){0}, (t_vec){0}, 0, 0});
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
	float	dist;
	float	temp;
	t_vec	normal;

	dist = -1;
	while (wpath)
	{
		if (((t_wpath *)wpath->content)->mode == 0)
			temp = intersect_segment(ray->origin, ray->dir,
				line(ray->real, *(t_wpath *)wpath->content));
		else
			temp = intersect_arc(ray->origin, ray->dir,
				line(ray->real, *(t_wpath *)wpath->content), &normal);
		if (temp < 0)
		{
			wpath = wpath->next;
			continue ;
		}
		if (temp < dist || dist == -1)
		{
			*wall = *(t_wpath *)wpath->content;
			if (((t_wpath *)wpath->content)->mode == 1)
				wall->normal = normal;
			ray->angle_factor = angle(ray->dir, wall->normal);
			dist = temp;
		}
		wpath = wpath->next;
	}
	return (is_valid_hit(ray, dist));
}
