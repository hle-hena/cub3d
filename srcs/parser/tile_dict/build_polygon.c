/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_polygon.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 10:55:56 by hle-hena          #+#    #+#             */
/*   Updated: 2025/07/27 22:31:19 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	debug = 0;

void	print_graph(t_graph *graph, char *letter)
{
	int	i;

	i = -1;
	printf("This is the graph of %s:\n", letter);
	while (++i < graph->nb_links)
	{
		if (graph->links[i].type == 0)
		{
			printf("\tA line is connecting {%.12f, %.12f} to {%.12f, %.12f}\n",
				graph->links[i].start->coo.x, graph->links[i].start->coo.y,
				graph->links[i].end->coo.x, graph->links[i].end->coo.y);
		}
		else
		{
			printf("\tAn arc is connecting {%.12f, %.12f} to {%.12f, %.12f}\
, with {%.12f, %.12f} as center\n",
				graph->links[i].start->coo.x, graph->links[i].start->coo.y,
				graph->links[i].end->coo.x, graph->links[i].end->coo.y,
				graph->links[i].center.x, graph->links[i].center.y);
		}
	}
	// printf("\tStarting to grow the graph.\n");
}

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

static inline t_vec	vec_unit(t_vec a)
{
	return (vec_scale(a, 1 / sqrtf(vec_len2(a))));
}

int	find_node(t_node *nodes, t_vec coo, int *current, int should_create)
{
	int	i;

	i = -1;
	while (++i < *current)
	{
		if (fabs(nodes[i].coo.x - coo.x) < 1e-6f
			&& fabs(nodes[i].coo.y - coo.y) < 1e-6f)
			return (i);
	}
	if (*current == 1023 || !should_create)
		return (-1);
	nodes[i].coo = coo;
	nodes[i].visited = 0;
	nodes[i].connect[0] = NULL;
	++(*current);
	return (i);
}

int	on_arc(t_link *C, t_vec hit, t_inter *inter, int can_hit_border)
{
	t_vec	v_hit;
	t_vec	v_end;
	t_vec	v_start;
	int		on_arc;
	float	angle;
	float	total_angle;

	(void)can_hit_border;
	v_hit = vec_unit(vec_sub(hit, C->center));
	v_end = vec_unit(vec_sub(C->end->coo, C->center));
	v_start = vec_unit(vec_sub(C->start->coo, C->center));
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

	if (check.t < 0.0f || check.t > 1.0f)
		return ;
	hit = vec_add(seg->start->coo, vec_scale(check.dir, check.t));
	v_hit = vec_unit(vec_sub(hit, arc->center));
	v_end = vec_unit(vec_sub(arc->end->coo, arc->center));
	v_start = vec_unit(vec_sub(arc->start->coo, arc->center));
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
	v_hit = vec_unit(vec_sub(hit, arc->center));
	v_end = vec_unit(vec_sub(arc->end->coo, arc->center));
	v_start = vec_unit(vec_sub(arc->start->coo, arc->center));
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

void	add_connection(t_node *dest, t_link *to_add)
{
	int	i;

	i = 0;
	while (dest->connect[i] && i < 511)
		++i;
	dest->connect[i] = to_add;
	dest->connect[i + 1] = NULL;
}

void	replace_connection(t_node *dest, t_link *to_replace, t_link *to_add)
{
	int	i;

	i = 0;
	while (dest->connect[i] != to_replace && i < 511)
		++i;
	dest->connect[i] = to_add;
}

void	build_primitive_graph(t_list *wpath, t_graph *graph, char *letter)
{
	t_wpath	*wall;
	int		j;
	int		found_start;
	int		found_end;

	j = -1;
	graph->nb_links = 0;
	graph->nb_nodes = 0;
	while (wpath && j < 511)
	{
		wall = (t_wpath *)wpath->content;
		found_start = find_node(graph->nodes, wall->start, &graph->nb_nodes, 1);
		found_end = find_node(graph->nodes, wall->end, &graph->nb_nodes, 1);
		graph->links[++j].start = &graph->nodes[found_start];
		graph->links[j].end = &graph->nodes[found_end];
		graph->links[j].center = wall->center;
		graph->links[j].type = wall->mode;
		graph->links[j].reflectance = wall->reflectance;
		graph->links[j].visited[0] = 0;
		graph->links[j].visited[1] = 0;
		add_connection(&graph->nodes[found_start], &graph->links[j]);
		if (found_end != found_start)
			add_connection(&graph->nodes[found_end], &graph->links[j]);
		wpath = wpath->next;
		++graph->nb_links;
	}
	if (debug)
		print_graph(graph, letter);
	if (wpath)
		return (printf("This is an error and you forgot it, you little angel\n"), VOID);//This means that we got too many lines for that tile.
}

//this code suppose that we don't have duplicate of circles and lines, I need to do that.
void	grow_graph(t_graph *graph, char *letter)
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
			// printf("\t\tSplitting the segment index [%d] at the coo {%.12f, %.12f}\n", i, graph->nodes[i_node].coo.x, graph->nodes[i_node].coo.y);
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
			if (debug)
				print_graph(graph, letter);
		}
	}
	//should do something if it exit because of overflow
}

void	generate_angles(t_node *current, float *angles)
{
	int		j;
	t_vec	out;
//need to normalise the second point for the circles so that if they are not on the circle, I place them on.
//yeah, but why ? I forgot ...
//it is for the else of the arc. if the point isnt really the end points, it will fuck things up.
	j = -1;
	while (current->connect[++j])
	{
		if (current->connect[j]->type == 0
			&& current->connect[j]->start == current)
			out = (t_vec){current->connect[j]->end->coo.x - current->coo.x,
				current->connect[j]->end->coo.y - current->coo.y};
		else if (current->connect[j]->type == 0)
			out = (t_vec){current->connect[j]->start->coo.x - current->coo.x,
				current->connect[j]->start->coo.y - current->coo.y};
		else
		{
			t_vec radius = {
				current->coo.x - current->connect[j]->center.x,
				current->coo.y - current->connect[j]->center.y
			};
			out = (t_vec){-radius.y, radius.x};
			if (current->connect[j]->end == current)
				out = (t_vec){-out.x, -out.y};
		}
		angles[j] = atan2(out.y, out.x);
	}
}

void	switch_two(t_node *node, float *angles, int k)
{
	float	temp_f;
	void	*temp_p;

	temp_f = angles[k];
	angles[k] = angles[k + 1];
	angles[k + 1] = temp_f;
	temp_p = node->connect[k];
	node->connect[k] = node->connect[k + 1];
	node->connect[k + 1] = temp_p;
}

void	sort_edges(t_graph *graph)
{
	int		i;
	int		j;
	int		k;
	float	angles[512];

	i = -1;
	while (++i < graph->nb_nodes)
	{
		generate_angles(&graph->nodes[i], angles);
		j = -1;
		while (graph->nodes[i].connect[++j + 1])
		{
			k = -1;
			while (graph->nodes[i].connect[++k + j + 1])
			{
				if (angles[k] > angles[k + 1])
					switch_two(&graph->nodes[i], angles, k);
			}
		}
	}
}

int	find_unvisited_edge(t_graph *graph, int *index)
{
	int	i;

	i = -1;
	while (++i < graph->nb_links)
	{
		if (graph->links[i].visited[0] == 0
			|| graph->links[i].visited[1] == 0)
		{
			printf("Finding for adress %p\n", &graph->links[i]);
			*index = i;
			return (0);
		}
	}
	return (1);
}

int	find_link(t_graph *graph, t_link *link)
{
	int	i;

	i = -1;
	while (++i < graph->nb_links)
	{
		if (graph->links[i].type == 0 && graph->links[i].start == link->start
			&& graph->links[i].end == link->end && link->type == 0)
			return (i);
		else if (link->type == 1 &&graph->links[i].start == link->start
			&& graph->links[i].end == link->end && graph->links[i].type == 1
			&& fabs(graph->links[i].center.x - link->center.x) < 1e-6
			&& fabs(graph->links[i].center.y - link->center.y) < 1e-6)
			return (i);
	}
	return  (0);
}

void	copy_link(t_graph *to, t_link *src, t_node *current_node)
{
	int		found_start;
	int		found_end;

	found_start = find_node(to->nodes, src->start->coo,
		&to->nb_nodes, 1);
	found_end = find_node(to->nodes, src->end->coo,
		&to->nb_nodes, 1);
	to->links[to->nb_links] = *src;
	to->links[to->nb_links].run_forward = 1;
	if (src->start == current_node)
	{
		src->visited[0] = 1;
		if (src->start == src->end)
			src->visited[1] = 1;
		to->links[to->nb_links].start = &to->nodes[found_start];
		to->links[to->nb_links].end = &to->nodes[found_end];
	}
	else
	{
		src->visited[1] = 1;
		if (src->start == src->end)
			src->visited[0] = 1;
		if (src->type == 1)
		{
			to->links[to->nb_links].run_forward = 0;
			to->links[to->nb_links].start = &to->nodes[found_start];
			to->links[to->nb_links].end = &to->nodes[found_end];
		}
		else
		{
			to->links[to->nb_links].start = &to->nodes[found_end];
			to->links[to->nb_links].end = &to->nodes[found_start];
		}
	}
	to->nb_links++;
}

void	get_subgraph(t_graph *graph, t_graph *subgraph, int start_edge)
{
	t_link	*current_link;
	t_node	*current_node;
	t_node	*start_node;
	int		i;

	current_link = &graph->links[start_edge];
	if (!current_link->visited[0])
	{
		start_node = current_link->start;
		current_node = current_link->end;
	}
	else
	{
		current_node = current_link->start;
		start_node = current_link->end;
	}
	copy_link(subgraph, &graph->links[start_edge], start_node);
	while (current_node != start_node)
	{
		i = 0;
		while (current_node->connect[i] != current_link)
			i++;
		current_link = current_node->connect[++i];
		if (!current_link)
			current_link = current_node->connect[0];
		copy_link(subgraph, current_link, current_node);
		if (current_node == current_link->start)
			current_node = current_link->end;
		else
			current_node = current_link->start;
	}
}

float	angle_from_center(t_vec center, t_vec point)
{
	return atan2f(point.y - center.y, point.x - center.x);
}

float	area_link(t_link *link)
{
	t_vec	a;
	t_vec	b;
	t_vec	theta;
	float	r;
	float	dtheta;

	a = link->end->coo;
	b = link->start->coo;
	if (link->run_forward)
		a = link->start->coo;
	if (link->run_forward)
		b = link->end->coo;
	if (link->type == 0)
		return (0.5f * (a.x * b.y - b.x * a.y));
	r = sqrtf(vec_len2(vec_sub(a, link->center)));
	theta.x = angle_from_center(link->center, a);
	theta.y = angle_from_center(link->center, b);
	dtheta = theta.y - theta.x;
	if (dtheta <= 0)
		dtheta += 2.0f * PI;
	if (!link->run_forward)
		dtheta = - (2.0f * PI - dtheta);
	return (0.5f * (r * link->center.x * (sinf(theta.y) - sinf(theta.x))
			- r * link->center.y * (cosf(theta.y) - cosf(theta.x))
			+ r * r * dtheta));
}

float	polygon_area(t_graph *graph)
{
	int		i;
	float	area;

	area = 0;
	i = -1;
	while (++i < graph->nb_links)
		area += area_link(&graph->links[i]);
	return (area);
}

t_graph	*retrieve_outer_face(t_graph *graph, char *letter)
{
	int		index;
	t_graph	*temp_graph;

	temp_graph = malloc(sizeof(t_graph));
	if (!temp_graph)
		return (ft_perror(-1, "Internal error: malloc.", 0), NULL);
	sort_edges(graph);
	while (1)
	{
		if (find_unvisited_edge(graph, &index))
			break ;
		temp_graph->nb_links = 0;
		temp_graph->nb_nodes = 0;
		get_subgraph(graph, temp_graph, index);
		if (debug)
			print_graph(temp_graph, letter);
		if (polygon_area(temp_graph) >= 0)
			return (temp_graph);
	}
	ft_del((void **)&temp_graph);
	return (NULL);
}

int	build_polygon(t_tile *tile, char id)
{
	char	letter[2];
	t_graph	*graph;
	t_graph	*outer_face;

	letter[0] = id;
	letter[1] = 0;
	if (ft_lstsize(tile->wpath) >= 32)
		return (ft_perror(-1, ft_strsjoin((char *[]){"Too many lines for tile \
", letter, ". Expected at most 32 lines.", NULL}), 0), 1);
	graph = malloc(sizeof(t_graph));
	if (!graph)
		return (ft_perror(-1, "Internal error: malloc.", 0), 1);
	build_primitive_graph(tile->wpath, graph, letter);
	if (!debug)
		print_graph(graph, letter);
	grow_graph(graph, letter);
	outer_face = retrieve_outer_face(graph, letter);
	if (outer_face && !debug)
	{
		print_graph(outer_face, letter);
		ft_del((void **)&outer_face);
	}
	ft_del((void **)&graph);
	return (0);
}

int	build_polygons()
{
	t_tile	**tiles;
	int		i;

	tiles = get_tile_dict();
	i = -1;
	while (++i < 255)
	{
		if (!tiles[i])
			continue ;
		if (build_polygon(tiles[i], i))
			return (1);
		// else
		// 	break ;
	}
	return (0);
}
