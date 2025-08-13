/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_slink.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:30:14 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/13 16:33:53 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_on_larc(t_info_arc info, t_link *seg, t_inter *out, t_graph *graph)
{
	int		on_arc;
	int		found;
	float	angle;

	if (out->dist < 0 - 1e-4f || out->dist > 1.0f + 1e-4f)
		return (0);
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
	found = find_node(graph->nodes, info.hit, &graph->nb_nodes, 0);
	if (found == -1 || (&graph->nodes[found] != seg->start
		&& &graph->nodes[found] != seg->end))
		on_arc = 0;
	if (on_arc)
		*out = (t_inter){info.hit, info.v_hit, angle, out->dist};
	return (on_arc);
}

t_inter	check_solutions_slarc(float *t, t_link *seg, t_link *arc, t_graph *graph)
{
	t_info_arc	info;
	t_inter		out1;
	t_inter		out2;
	t_vec		dir;

	//this function has a bug if you are inside of the arc or maybe just inside the tile ? It makes it do weird thing.
	dir = vec_sub(seg->end->coo, seg->start->coo);
	info = (t_info_arc){(t_vec){0}, vec_normalize(vec_sub(arc->end->coo, arc->center)),
		vec_normalize(vec_sub(arc->start->coo, arc->center)), (t_vec){0}, 0, 0,
		arc->run_forward};
	info.is_full_circle = fabsf(arc->start->coo.x - arc->end->coo.x) < FLT_EPSILON
		&& fabsf(arc->start->coo.y - arc->end->coo.y) < FLT_EPSILON;
	info.total_angle = acosf(vec_dot(info.v_start, info.v_end));
	if (vec_cross(info.v_start, info.v_end) < 0)
		info.total_angle = (2.0f * PI) - info.total_angle;
	out1 = (t_inter){(t_vec){0}, (t_vec){0}, -1, t[0]};
	out2 = (t_inter){(t_vec){0}, (t_vec){0}, -1, t[1]};
	info.hit = vec_add(seg->start->coo, vec_scale(dir, out1.dist));
	info.v_hit = vec_normalize(vec_sub(info.hit, arc->center));
	if (is_on_larc(info, seg, &out1, graph) && out1.dist < out2.dist)
		return (out1);
	info.hit = vec_add(seg->start->coo, vec_scale(dir, out2.dist));
	info.v_hit = vec_normalize(vec_sub(info.hit, arc->center));
	if (is_on_larc(info, seg, &out2, graph))
		return (out2);
	return (out1);
}

t_inter	intersect_alseg(t_link *seg, t_link *arc, t_graph *graph)
{
	t_vec	delta;
	t_vec	dir;
	float	a;
	float	b;
	float	discriminant;

	dir = vec_sub(seg->end->coo, seg->start->coo);
	delta = vec_sub(seg->start->coo, arc->center);
	a = vec_dot(dir, dir);
	b = 2.0f * vec_dot(dir, delta);
	discriminant = (b * b) - 4.0f * a * (vec_dot(delta, delta)
			- vec_len2(vec_sub(arc->start->coo, arc->center)));
	if (discriminant < 0.0f)
		return ((t_inter){(t_vec){0}, (t_vec){0}, -1, -1});
	discriminant = sqrtf(discriminant);
	return (check_solutions_slarc((float [2]){(-b - discriminant) / (2.0f * a),
			(-b + discriminant) / (2.0f * a)}, seg, arc, graph));
}

t_inter	intersect_slseg(t_link *seg1, t_link *seg2, t_graph *graph)
{
	t_vec	v1;
	t_vec	v2;
	float	s;
	float	t;
	int		found;

	v1 = (t_vec){seg1->end->coo.x - seg1->start->coo.x,
		seg1->end->coo.y - seg1->start->coo.y};
	v2 = (t_vec){seg2->end->coo.x - seg2->start->coo.x,
		seg2->end->coo.y - seg2->start->coo.y};
	s = (-v1.y * (seg1->start->coo.x - seg2->start->coo.x) + v1.x
			* (seg1->start->coo.y - seg2->start->coo.y))
		/ (-v2.x * v1.y + v1.x * v2.y);
	t = (v2.x * (seg1->start->coo.y - seg2->start->coo.y) - v2.y
			* (seg1->start->coo.x - seg2->start->coo.x))
		/ (-v2.x * v1.y + v1.x * v2.y);
	if (s < 0 || s > 1 || t <= 0 || t >= 1)
		return ((t_inter){(t_vec){0}, (t_vec){0}, -1, -1});
	v2.x = seg1->start->coo.x + (t * v1.x);
	v2.y = seg1->start->coo.y + (t * v1.y);
	found = find_node(graph->nodes, v2, &graph->nb_nodes, 0);
	if (found == -1 || (&graph->nodes[found] != seg1->start
			&& &graph->nodes[found] != seg1->end))
		return ((t_inter){v2, (t_vec){0}, 0, t});
	return ((t_inter){(t_vec){0}, (t_vec){0}, -1, -1});
}
