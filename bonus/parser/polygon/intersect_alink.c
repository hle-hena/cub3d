/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_alink.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:30:35 by hle-hena          #+#    #+#             */
/*   Updated: 2025/09/11 14:50:35 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

int	on_larc(t_info_arc info, t_inter *out)
{
	int		on_arc;
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
	if (on_arc)
		*out = (t_inter){info.hit, info.v_hit, angle, out->dist};
	return (on_arc);
}

static int	retrieve_utils_vec(t_link *arc1, t_link *arc2, t_vec *p_mid,
	t_vec *perp)
{
	float	d;
	float	a;
	float	h;
	float	r[2];
	t_vec	d_vec;

	r[0] = sqrtf(vec_len2(vec_sub(arc1->start->coo, arc1->center)));
	r[1] = sqrtf(vec_len2(vec_sub(arc2->start->coo, arc2->center)));
	d_vec = vec_sub(arc2->center, arc1->center);
	d = sqrtf(vec_len2(d_vec));
	if (d > r[0] + r[1] || d < fabsf(r[0] - r[1]) || d == 0.0f)
		return (1);
	a = (r[0] * r[0] - r[1] * r[1] + d * d) / (2.0f * d);
	h = sqrtf(r[0] * r[0] - a * a);
	*p_mid = vec_add(arc1->center, vec_scale(d_vec, a / d));
	*perp = (t_vec){-d_vec.y * (h / d), d_vec.x * (h / d)};
	return (0);
}

void	test_potential_hit_alarc(t_info_arc *info_arc, t_inter *inter,
	t_link *arc1, t_graph *graph)
{
	int	found;

	info_arc[0].v_hit = vec_normalize(vec_sub(info_arc[0].hit, arc1->center));
	info_arc[1].hit = info_arc[0].hit;
	inter[0].pos = -1;
	inter[1].pos = -1;
	if (on_larc(info_arc[0], &inter[0]) && on_larc(info_arc[1], &inter[1]))
	{
		if (inter[0].pos > -0.5f && inter[1].pos > -0.5f
			&& (inter[0].pos < inter[2].dist || (inter[2].dist < 0)))
		{
			found = find_node(graph->nodes, inter[0].coo, &graph->nb_nodes, 0);
			if (found == -1 || (&graph->nodes[found] != arc1->start
					&& &graph->nodes[found] != arc1->end))
				inter[2] = (t_inter){inter[0].coo, (t_vec){0}, 0, inter[0].pos};
		}
	}
}

t_inter	intersect_alarc(t_link *arc1, t_link *arc2, t_graph *graph)
{
	t_vec			perp;
	t_vec			p_mid;
	t_inter			inter[3];
	t_info_arc		info_arc[2];

	inter[2].dist = -1;
	info_arc[0] = (t_info_arc){(t_vec){0}, vec_normalize(vec_sub(arc1->end->coo,
				arc1->center)), vec_normalize(vec_sub(arc1->start->coo,
				arc1->center)), (t_vec){0}, 0, (arc1->start->coo.x
			== arc1->end->coo.x && arc1->start->coo.y == arc1->end->coo.y),
		arc1->run_forward};
	info_arc[1] = (t_info_arc){(t_vec){0}, vec_normalize(vec_sub(arc2->end->coo,
				arc2->center)), vec_normalize(vec_sub(arc2->start->coo,
				arc2->center)), (t_vec){0}, 0, (arc2->start->coo.x
			== arc2->end->coo.x && arc2->start->coo.y == arc2->end->coo.y),
		arc2->run_forward};
	if (retrieve_utils_vec(arc1, arc2, &p_mid, &perp))
		return ((t_inter){(t_vec){0}, (t_vec){0}, -1, -1});
	info_arc[0].hit = vec_add(p_mid, perp);
	info_arc[1].v_hit = vec_normalize(vec_sub(info_arc[0].hit, arc2->center));
	test_potential_hit_alarc(info_arc, inter, arc1, graph);
	info_arc[0].hit = vec_sub(p_mid, perp);
	info_arc[1].v_hit = vec_normalize(vec_sub(info_arc[0].hit, arc2->center));
	test_potential_hit_alarc(info_arc, inter, arc1, graph);
	return (inter[2]);
}
