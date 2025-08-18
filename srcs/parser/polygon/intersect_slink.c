/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_slink.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:30:14 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/18 13:25:07 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_on_larc(t_info_arc info, t_link *edge_check, t_inter *out,
	t_graph *graph)
{
	int		on_arc;
	int		found;
	float	angle;

	if (out->dist < 0 || out->dist > 1.0f)
		return (0);
	if (info.is_full_circle)
	{
		found = find_node(graph->nodes, info.hit, &graph->nb_nodes, 0);
		if (!(found == -1 || (&graph->nodes[found] != edge_check->start
					&& &graph->nodes[found] != edge_check->end)))
			return (0);
		angle = get_arc_angle(info.v_start, info.v_hit, 2.0f * PI);
		*out = (t_inter){info.hit, info.v_hit, angle, out->dist};
		return (1);
	}
	on_arc = is_on_arc_sweep(info);
	angle = get_arc_angle(info.v_start, info.v_hit, info.total_angle);
	found = find_node(graph->nodes, info.hit, &graph->nb_nodes, 0);
	on_arc = on_arc * (found == -1 || (&graph->nodes[found] != edge_check->start
				&& &graph->nodes[found] != edge_check->end));
	if (on_arc)
		*out = (t_inter){info.hit, info.v_hit, angle, out->dist};
	return (on_arc);
}

t_inter	check_solutions_slarc(float *t, t_link *seg, t_link *arc,
	t_graph *graph)
{
	t_info_arc	info;
	t_inter		out[2];
	t_vec		dir;

	dir = vec_sub(seg->end->coo, seg->start->coo);
	info = (t_info_arc){(t_vec){0}, vec_normalize(vec_sub(arc->end->coo,
				arc->center)), vec_normalize(vec_sub(arc->start->coo,
				arc->center)), (t_vec){0}, 0, 0, arc->run_forward};
	info.is_full_circle = arc->start->coo.x == arc->end->coo.x
		&& arc->start->coo.y == arc->end->coo.y;
	info.total_angle = acosf(vec_dot(info.v_start, info.v_end));
	if (vec_cross(info.v_start, info.v_end) < 0)
		info.total_angle = (2.0f * PI) - info.total_angle;
	out[0] = (t_inter){(t_vec){0}, (t_vec){0}, -1, t[0]};
	out[1] = (t_inter){(t_vec){0}, (t_vec){0}, -1, t[1]};
	info.hit = vec_add(seg->start->coo, vec_scale(dir, out[0].dist));
	info.v_hit = vec_normalize(vec_sub(info.hit, arc->center));
	is_on_larc(info, seg, &out[0], graph);
	info.hit = vec_add(seg->start->coo, vec_scale(dir, out[1].dist));
	info.v_hit = vec_normalize(vec_sub(info.hit, arc->center));
	is_on_larc(info, seg, &out[1], graph);
	if (out[0].pos < 0 && out[1].pos < 0)
		return ((t_inter){(t_vec){0}, (t_vec){0}, -1, -1});
	return (out[!(out[0].pos > -0.5f
				&& (out[0].dist < out[1].dist || out[1].pos < 0))]);
}

t_inter	check_solutions_alseg(float *t, t_link *seg, t_link *arc,
	t_graph *graph)
{
	t_info_arc	info;
	t_inter		out[2];
	t_vec		dir;

	dir = vec_sub(seg->end->coo, seg->start->coo);
	info = (t_info_arc){(t_vec){0}, vec_normalize(vec_sub(arc->end->coo,
				arc->center)), vec_normalize(vec_sub(arc->start->coo,
				arc->center)), (t_vec){0}, 0, 0, arc->run_forward};
	info.is_full_circle = arc->start->coo.x == arc->end->coo.x
		&& arc->start->coo.y == arc->end->coo.y;
	info.total_angle = acosf(vec_dot(info.v_start, info.v_end));
	if (vec_cross(info.v_start, info.v_end) < 0)
		info.total_angle = (2.0f * PI) - info.total_angle;
	out[0] = (t_inter){(t_vec){0}, (t_vec){0}, -1, t[0]};
	out[1] = (t_inter){(t_vec){0}, (t_vec){0}, -1, t[1]};
	info.hit = vec_add(seg->start->coo, vec_scale(dir, out[0].dist));
	info.v_hit = vec_normalize(vec_sub(info.hit, arc->center));
	is_on_larc(info, arc, &out[0], graph);
	info.hit = vec_add(seg->start->coo, vec_scale(dir, out[1].dist));
	info.v_hit = vec_normalize(vec_sub(info.hit, arc->center));
	is_on_larc(info, arc, &out[1], graph);
	if ((out[0].pos < 0 && out[1].pos < 0))
		return ((t_inter){(t_vec){0}, (t_vec){0}, -1, -1});
	return (out[!(out[0].pos > -0.5f
				&& (out[0].pos < out[1].pos || out[1].pos < 0))]);
}

t_inter	intersect_alsl(t_link *seg, t_link *arc, t_graph *graph,
		t_inter (*check)(float *, t_link *, t_link *, t_graph *))
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
	return (check((float [2]){(-b - discriminant) / (2.0f * a),
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
