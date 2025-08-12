/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grow_graph.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:25:41 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/12 17:29:57 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	on_arc(t_link *C, t_vec hit, t_inter *inter, int can_hit_border)
{
	t_vec	v_hit;
	t_vec	v_end;
	t_vec	v_start;
	int		on_arc;
	float	angle;
	float	total_angle;

	(void)can_hit_border;
	v_hit = vec_normalize(vec_sub(hit, C->center));
	v_end = vec_normalize(vec_sub(C->end->coo, C->center));
	v_start = vec_normalize(vec_sub(C->start->coo, C->center));
	if (C->start->coo.x == C->end->coo.x && C->start->coo.y == C->end->coo.y)
	{
		angle = acosf(vec_dot(v_start, v_hit)) / (2.0f * PI);
		if (vec_cross(v_start, v_hit) < 0)
			angle = 1.0f - angle;
		inter->coo = hit;
		inter->dist = angle;
		return (1);
	}
	if (vec_cross(v_start, v_end) >= 0)
		on_arc = (vec_cross(v_start, v_hit) >= -FLT_EPSILON
			&& vec_cross(v_hit, v_end) >= -FLT_EPSILON);
	else
		on_arc = (vec_cross(v_start, v_hit) >= -FLT_EPSILON
			|| vec_cross(v_hit, v_end) >= -FLT_EPSILON);
	angle = acosf(fminf(fmaxf(vec_dot(v_start, v_hit), -1.0f), 1.0f));
	if (vec_cross(v_start, v_hit) < 0)
		angle = (2.0f * PI) - angle;
	total_angle = acosf(vec_dot(v_start, v_end));
	if (vec_cross(v_start, v_end) < 0)
		total_angle = (2.0f * PI) - total_angle;
	angle /= total_angle;
	if (on_arc)
	{
		inter->coo = hit;
		inter->dist = angle;
		return (1);
	}
	return (0);
}

int	retrieve_utils_vec(t_link *arc1, t_link *arc2, t_vec *p_mid, t_vec *perp)
{
	float	r1;
	float	r2;
	float	d;
	float	a;
	float	h;
	t_vec	d_vec;

	r1 = sqrtf(vec_len2(vec_sub(arc1->start->coo, arc1->center)));
	r2 = sqrtf(vec_len2(vec_sub(arc2->start->coo, arc2->center)));
	d_vec = vec_sub(arc2->center, arc1->center);
	d = sqrtf(vec_len2(d_vec));
	if (d > r1 + r2 || d < fabsf(r1 - r2) || d == 0.0f)
		return (1);
	a = (r1 * r1 - r2 * r2 + d * d) / (2.0f * d);
	h = sqrtf(r1 * r1 - a * a);
	*p_mid = vec_add(arc1->center, vec_scale(d_vec, a / d));
	*perp = (t_vec){-d_vec.y * (h / d), d_vec.x * (h / d)};
	return (0);
}

void	intersect_arc_arc(t_link *arc1, t_link *arc2, t_inter *inter, t_graph *graph)
{
	t_vec			check;
	t_vec			perp;
	t_vec			p_mid;
	t_inter			trash;
	t_inter			temp;
	int				found;

	temp.dist = -1;
	trash.dist = -1;
	if (retrieve_utils_vec(arc1, arc2, &p_mid, &perp))
		return ;
	check = vec_add(p_mid, perp);
	if (on_arc(arc1, check, &temp, 0) && on_arc(arc2, check, &trash, 1))
	{
		if (temp.dist < inter->dist || inter->dist < 0)
		{
			found = find_node(graph->nodes, temp.coo, &graph->nb_nodes, 0);
			if (found == -1 || (&graph->nodes[found] != arc1->start
				&& &graph->nodes[found] != arc1->end))
			{
				inter->dist = temp.dist;
				inter->coo = temp.coo;
			}
		}
	}
	check = vec_sub(p_mid, perp);
	if (on_arc(arc1, check, &temp, 0) && on_arc(arc2, check, &trash, 1))
	{
		if (temp.dist < inter->dist || inter->dist < 0)
		{
			found = find_node(graph->nodes, temp.coo, &graph->nb_nodes, 0);
			if (found == -1 || (&graph->nodes[found] != arc1->start
				&& &graph->nodes[found] != arc1->end))
			{
				inter->dist = temp.dist;
				inter->coo = temp.coo;
			}
		}
	}
}

void	check_possible_arc_seg(t_link *seg, t_link *arc, t_info_check check, t_graph *graph)
{
	t_vec	hit;
	t_vec	v_hit;
	t_vec	v_end;
	t_vec	v_start;
	int		on_arc;
	float	angle;
	float	total_angle;
	int		found;

	if (check.t < 0 - 1e-4f || check.t > 1.0f + 1e-4f)
		return ;
	hit = vec_add(seg->start->coo, vec_scale(check.dir, check.t));
	v_hit = vec_normalize(vec_sub(hit, arc->center));
	v_end = vec_normalize(vec_sub(arc->end->coo, arc->center));
	v_start = vec_normalize(vec_sub(arc->start->coo, arc->center));
	if (arc->start->coo.x == arc->end->coo.x && arc->start->coo.y == arc->end->coo.y)
	{
		angle = acosf(vec_dot(v_start, v_hit)) / (2.0f * PI);
		if (vec_cross(v_start, v_hit) < 0)
			angle = 1.0f - angle;
		if ((angle < check.inter->dist || check.inter->dist < 0))
		{
			found = find_node(graph->nodes, hit, &graph->nb_nodes, 0);
			if (found == -1 || (&graph->nodes[found] != arc->start
				&& &graph->nodes[found] != arc->end))
			{
				check.inter->coo = hit;
				check.inter->dist = angle;
			}
		}
		return ;
	}
	if (vec_cross(v_start, v_end) >= 0)
		on_arc = (vec_cross(v_start, v_hit) >= -FLT_EPSILON
			&& vec_cross(v_hit, v_end) >= -FLT_EPSILON);
	else
		on_arc = (vec_cross(v_start, v_hit) >= -FLT_EPSILON
			|| vec_cross(v_hit, v_end) >= -FLT_EPSILON);
	angle = acosf(fminf(fmaxf(vec_dot(v_start, v_hit), -1.0f), 1.0f));
	if (vec_cross(v_start, v_hit) < 0)
		angle = (2.0f * PI) - angle;
	total_angle = acosf(vec_dot(v_start, v_end));
	if (vec_cross(v_start, v_end) < 0)
		total_angle = (2.0f * PI) - total_angle;
	angle /= total_angle;
	if (on_arc && (angle < check.inter->dist || check.inter->dist < 0))
	{
		found = find_node(graph->nodes, hit, &graph->nb_nodes, 0);
		if (found == -1 || (&graph->nodes[found] != arc->start
			&& &graph->nodes[found] != arc->end))
		{
			check.inter->coo = hit;
			check.inter->dist = angle;
		}
	}
}

void	check_possible_seg_arc(t_link *seg, t_link *arc, t_info_check check, t_graph *graph)
{
	t_vec	hit;
	t_vec	v_hit;
	t_vec	v_end;
	t_vec	v_start;
	int		on_arc;
	int		found;

	if (check.t < 0.0f || check.t > 1.0f)
		return ;
	hit = vec_add(seg->start->coo, vec_scale(check.dir, check.t));
	if (arc->start->coo.x == arc->end->coo.x && arc->start->coo.y == arc->end->coo.y)
	{
		if ((check.t < check.inter->dist || check.inter->dist < 0))
		{
			found = find_node(graph->nodes, hit, &graph->nb_nodes, 0);
			if (found == -1 || (&graph->nodes[found] != seg->start
				&& &graph->nodes[found] != seg->end))
			{
				check.inter->coo = hit;
				check.inter->dist = check.t;
			}
		}
		return ;
	}
	v_hit = vec_normalize(vec_sub(hit, arc->center));
	v_end = vec_normalize(vec_sub(arc->end->coo, arc->center));
	v_start = vec_normalize(vec_sub(arc->start->coo, arc->center));
	if (vec_cross(v_start, v_end) >= 0)
		on_arc = (vec_cross(v_start, v_hit) >= -FLT_EPSILON
			&& vec_cross(v_hit, v_end) >= -FLT_EPSILON);
	else
		on_arc = (vec_cross(v_start, v_hit) >= -FLT_EPSILON
			|| vec_cross(v_hit, v_end) >= -FLT_EPSILON);
	if (on_arc && (check.t < check.inter->dist || check.inter->dist < 0))
	{
		found = find_node(graph->nodes, hit, &graph->nb_nodes, 0);
		if (found == -1 || (&graph->nodes[found] != seg->start
			&& &graph->nodes[found] != seg->end))
		{
			check.inter->coo = hit;
			check.inter->dist = check.t;
		}
	}
}

void	intersect_seg_arc(t_link *seg, t_link *arc, t_inter *inter, void (*f)(t_link *, t_link *, t_info_check, t_graph *), t_graph *graph)
{
	t_vec	delta;
	t_vec	dir;
	float	a;
	float	b;
	float	discriminant;
	float	sqrt_d;

	dir = vec_sub(seg->end->coo, seg->start->coo);
	delta = vec_sub(seg->start->coo, arc->center);
	a = vec_dot(dir, dir);
	b = 2.0f * vec_dot(dir, delta);
	discriminant = (b * b) - 4.0f * a * (vec_dot(delta, delta) -
			vec_len2(vec_sub(arc->start->coo, arc->center)));
	if (discriminant < 0.0f)
		return;
	sqrt_d = sqrtf(discriminant);
	f(seg, arc, (t_info_check){inter, dir, (-b - sqrt_d) / (2.0f * a)}, graph);
	f(seg, arc, (t_info_check){inter, dir, (-b + sqrt_d) / (2.0f * a)}, graph);
}

void	intersect_seg_seg(t_link *l1, t_link *l2, t_inter *inter, t_graph *graph)
{
	t_vec	s1;
	t_vec	s2;
	float	s;
	float	t;
	int		found;

	s1.x = l1->end->coo.x - l1->start->coo.x;
	s1.y = l1->end->coo.y - l1->start->coo.y;
	s2.x = l2->end->coo.x - l2->start->coo.x;
	s2.y = l2->end->coo.y - l2->start->coo.y;
	s = (-s1.y * (l1->start->coo.x - l2->start->coo.x) + s1.x
		* (l1->start->coo.y - l2->start->coo.y)) / (-s2.x * s1.y + s1.x * s2.y);
	t = ( s2.x * (l1->start->coo.y - l2->start->coo.y) - s2.y
		* (l1->start->coo.x - l2->start->coo.x)) / (-s2.x * s1.y + s1.x * s2.y);
	if (s >= 0 && s <= 1 && t > 0 && t < 1 && (t < inter->dist || inter->dist < 0))
	{
		s2.x = l1->start->coo.x + (t * s1.x);
		s2.y = l1->start->coo.y + (t * s1.y);
		found = find_node(graph->nodes, s2, &graph->nb_nodes, 0);
		if (found == -1 || (&graph->nodes[found] != l1->start
			&& &graph->nodes[found] != l1->end))
		{
			inter->coo = s2;
			inter->dist = t;
		}
	}
}

void	intersect_switch(t_link *l1, t_link *l2, t_inter *inter, t_graph *graph)
{
	if (l1->type == 0 && l2->type == 0)
		intersect_seg_seg(l1, l2, inter, graph);
	else if (l1->type == 0)
		intersect_seg_arc(l1, l2, inter, &check_possible_seg_arc, graph);
	else if (l2->type == 0)
		intersect_seg_arc(l2, l1, inter, &check_possible_arc_seg, graph);
	else
		intersect_arc_arc(l1, l2, inter, graph);
}

int	find_closest_inter(t_graph *graph, int current)
{
	t_inter	inter;
	int		i;

	i = -1;
	inter.dist = -1;
	while (++i < graph->nb_links)
	{
		if (i == current)
			continue ;
		intersect_switch(&graph->links[current], &graph->links[i], &inter, graph);//might not need to pass it as a pointer ?
	}
	if (inter.dist == -1)
		return (-1);
	return (find_node(graph->nodes, inter.coo, &graph->nb_nodes, 1));
}

//this code suppose that we don't have duplicate of circles and lines, I need to do that.
int	grow_graph(t_graph *graph)
{
	int		i;
	int		i_node;

	//there might be an overflow possible ? //DEFINITLY POSSIBLE X)))))))))))))))))))))))))))))))))))))))))))))))))
	i = -1;
	while (++i < graph->nb_links && i < 511)
	{
		i_node = find_closest_inter(graph, i);
		if (i_node != -1)
		{
			graph->links[graph->nb_links].start = &graph->nodes[i_node];
			graph->links[graph->nb_links].end = graph->links[i].end;
			graph->links[graph->nb_links].center = graph->links[i].center;
			graph->links[graph->nb_links].type = graph->links[i].type;
			graph->links[graph->nb_links].reflectance = graph->links[i].reflectance;
			graph->links[graph->nb_links].visited[0] = 0;
			graph->links[graph->nb_links].visited[1] = 0;
			graph->links[i].end = &graph->nodes[i_node];
			if (graph->links[i].start != graph->links[graph->nb_links].end)
				replace_connection(graph->links[graph->nb_links].end,
					&graph->links[i], &graph->links[graph->nb_links]);
			else
				add_connection(graph->links[i].start, &graph->links[graph->nb_links]);
			add_connection(&graph->nodes[i_node], &graph->links[i]);
			add_connection(&graph->nodes[i_node], &graph->links[graph->nb_links]);
			++graph->nb_links;
		}
	}
	if (i == 512)
		return (ft_perror(-1, "There has been an overflow of the paths.", 0), 1);
	return (0);
}
