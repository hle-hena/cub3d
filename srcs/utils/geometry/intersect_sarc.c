/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_sarc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 10:58:37 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/12 17:34:30 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_on_arc(t_info_arc info, t_vec dir, t_inter *out)
{
	int		on_arc;
	int		inside_wall;
	float	angle;

	if (info.is_full_circle)
	{
		angle = acosf(vec_dot(info.v_start, info.v_hit)) / (2.0f * PI);
		if (vec_cross(info.v_start, info.v_hit) < 0)
			angle = 1.0f - angle;
		*out = (t_inter){info.hit, info.v_hit, angle, out->dist};
		return (1);
	}
	if (vec_cross(info.v_start, info.v_end) >= 0)
		on_arc = (vec_cross(info.v_start, info.v_hit) >= -FLT_EPSILON
				&& vec_cross(info.v_hit, info.v_end) >= -FLT_EPSILON);
	else
		on_arc = (vec_cross(info.v_start, info.v_hit) >= -FLT_EPSILON
				|| vec_cross(info.v_hit, info.v_end) >= -FLT_EPSILON);
	angle = acosf(fminf(fmaxf(vec_dot(info.v_start, info.v_hit), -1.0f), 1.0f));
	if (vec_cross(info.v_start, info.v_hit) < 0)
		angle = (2.0f * PI) - angle;
	angle /= info.total_angle;
	inside_wall = ((vec_dot(info.v_hit, dir) < 0) && !info.run_forward) * 1
		+ ((vec_dot(info.v_hit, dir) > 0) && info.run_forward) * -1;
	on_arc = on_arc && (inside_wall != 0);
	if (on_arc)
		*out = (t_inter){info.hit, info.v_hit, angle, out->dist};
	if (inside_wall == -1)
		out->normal = (t_vec){-out->normal.x, -out->normal.y};
	return (on_arc);
}

t_inter	check_solutions_sarc(float *t, t_vec origin, t_vec dir, t_wpath arc)
{
	t_info_arc	info;
	t_inter		out1;
	t_inter		out2;

	//this function has a bug if you are inside of the arc or maybe just inside the tile ? It makes it do weird thing.
	info = (t_info_arc){(t_vec){0}, vec_normalize(vec_sub(arc.end, arc.center)),
		vec_normalize(vec_sub(arc.start, arc.center)), (t_vec){0}, 0, 0,
		arc.run_forward};
	info.is_full_circle = fabsf(arc.start.x - arc.end.x) < FLT_EPSILON
		&& fabsf(arc.start.y - arc.end.y) < FLT_EPSILON;
	info.total_angle = acosf(vec_dot(info.v_start, info.v_end));
	if (vec_cross(info.v_start, info.v_end) < 0)
		info.total_angle = (2.0f * PI) - info.total_angle;
	out1 = (t_inter){(t_vec){0}, (t_vec){0}, -1, t[0]};
	out2 = (t_inter){(t_vec){0}, (t_vec){0}, -1, t[1]};
	info.hit = vec_add(origin, vec_scale(dir, out1.dist));
	info.v_hit = vec_normalize(vec_sub(info.hit, arc.center));
	if (is_on_arc(info, dir, &out1) && out1.dist < out2.dist)
		return (out1);
	info.hit = vec_add(origin, vec_scale(dir, out2.dist));
	info.v_hit = vec_normalize(vec_sub(info.hit, arc.center));
	if (is_on_arc(info, dir, &out2))
		return (out2);
	return (out1);
}
