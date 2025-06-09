/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_intersect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 13:37:04 by hle-hena          #+#    #+#             */
/*   Updated: 2025/06/09 19:08:49 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

static inline float intersect_arc(t_vec origin, t_vec dir, t_wpath arc,
	float *percent_out, t_vec *normal_out)
{
	t_vec delta = vec_sub(origin, arc.center);
	t_vec A = arc.start;
	t_vec B = arc.end;
	t_vec C = arc.center;

	t_vec v1 = vec_normalize(vec_sub(A, C)); // start
	t_vec v2 = vec_normalize(vec_sub(B, C)); // end
	float r2 = vec_len2(vec_sub(A, C));

	float a = vec_dot(dir, dir);
	float b = 2.0f * vec_dot(dir, delta);
	float c = vec_dot(delta, delta) - r2;
	float discriminant = b * b - 4.0f * a * c;
	if (discriminant < 0.0f)
		return -1.0f;

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

		// Handle full circle (A == B means 360°)
		if (fabsf(vec_dot(v1, v2) - 1.0f) < 1e-5f && vec_cross(v1, v2) < 1e-5f)
		{
			*normal_out = v_hit;
			*percent_out = acosf(vec_dot(v1, v_hit)) / (2.0f * M_PI);
			if (vec_cross(v1, v_hit) < 0)
				*percent_out = 1.0f - *percent_out;
			return t;
		}

		// Check if hit lies on arc (same logic as before)
		float cross_arc = vec_cross(v1, v2);
		float cross1 = vec_cross(v1, v_hit);
		float cross2 = vec_cross(v_hit, v2);
		int on_arc = (cross_arc >= 0) ? 
			(cross1 >= -FLT_EPSILON && cross2 >= -FLT_EPSILON) :
			(cross1 >= -FLT_EPSILON || cross2 >= -FLT_EPSILON);

		if (on_arc)
		{
			// Compute arc percent from v1 to v_hit
			float dot = vec_dot(v1, v_hit);
			dot = fminf(fmaxf(dot, -1.0f), 1.0f); // Clamp
			float angle = acosf(dot);
			if (vec_cross(v1, v_hit) < 0)
				angle = (2.0f * M_PI) - angle;
			float total_angle = acosf(vec_dot(v1, v2));
			if (vec_cross(v1, v2) < 0)
				total_angle = (2.0f * M_PI) - total_angle;

			*percent_out = angle / total_angle;
			*normal_out = v_hit;
			return t;
		}
	}
	return -1.0f;
}





static inline float	intersect_segment(t_vec origin, t_vec dir, t_wpath d,
	float *percent)
{
	t_vec	seg;
	float	det;
	float	t;
	float	u;

	seg = (t_vec){d.end.x - d.start.x, d.end.y - d.start.y};
	det = -dir.x * seg.y + dir.y * seg.x;
	if (fabs(det) < 1e-6f)
		return (-1);
	t_vec delta = {d.start.x - origin.x, d.start.y - origin.y};
	t = (delta.x * -seg.y + delta.y * seg.x) / det;
	u = (-dir.y * delta.x + dir.x * delta.y) / det;
	if (t >= 0 && u >= 0 && u <= 1)
		return (*percent = u, t);
	return (-1);
}

static inline t_wpath	line(t_point curr, t_wpath base)
{
	return ((t_wpath){(t_vec){base.start.x + curr.x, base.start.y + curr.y},
		(t_vec){base.end.x + curr.x, base.end.y + curr.y}, (t_vec){base.center.x
			+ curr.x, base.center.y + curr.y}, (t_text){0}, (t_vec){0}, 0, 0, 0});
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

int	does_hit(t_list	*wpath, t_ray *ray, t_wpath *wall)
{
	float	dist;
	float	temp;
	float	percent;
	t_vec	normal;

	dist = -1;
	while (wpath)
	{
		if (((t_wpath *)wpath->content)->mode == 0)
			temp = intersect_segment(ray->origin, ray->dir,
				line(ray->curr, *(t_wpath *)wpath->content), &percent);
		else
			temp = intersect_arc(ray->origin, ray->dir,
				line(ray->curr, *(t_wpath *)wpath->content), &percent, &normal);
		if (temp < 0)
		{
			wpath = wpath->next;
			continue ;
		}
		if (temp < dist || dist == -1)
		{
			*wall = *(t_wpath *)wpath->content;
			dist = temp;
			wall->pos = percent;
			if (((t_wpath *)wpath->content)->mode == 1)
				wall->normal = normal;
		}
		wpath = wpath->next;
	}
	// if (is_invalid_hit(ray, wall, dist))
	if (dist < 0)
		return (0);
	ray->precise_dist = dist;
	return (1);
}
