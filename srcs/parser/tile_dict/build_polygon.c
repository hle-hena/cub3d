/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_polygon.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 10:55:56 by hle-hena          #+#    #+#             */
/*   Updated: 2025/07/07 16:13:08 by hle-hena         ###   ########.fr       */
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

static inline t_vec	vec_unit(t_vec a)
{
	return (vec_scale(a, 1 / sqrtf(vec_len2(a))));
}

/* 
			float dot = vec_dot(v1, v_hit);
			dot = fminf(fmaxf(dot, -1.0f), 1.0f);
			float angle = acosf(dot);
			if (vec_cross(v1, v_hit) < 0)
				angle = (2.0f * M_PI) - angle;
			float total_angle = acosf(vec_dot(v1, v2));
			if (vec_cross(v1, v2) < 0)
				total_angle = (2.0f * M_PI) - total_angle;

			*percent_out = angle / total_angle;
			*normal_out = v_hit;
			return t;
 */

void	check_possible_arg_seg(t_link *seg, t_link *arc, t_info_check check)
{
	t_vec	hit;
	t_vec	v_hit;
	t_vec	v_end;
	t_vec	v_start;
	int		on_arc;
	float	angle;
	float	total_angle;

	if (check.t < 0.0f || check.t > 1.0f)
		return ;
	hit = vec_add(seg->start->coo, vec_scale(check.dir, check.t));
	v_hit = vec_unit(vec_sub(hit, arc->center));
	v_end = vec_unit(vec_sub(arc->end->coo, arc->center));
	v_start = vec_unit(vec_sub(arc->start->coo, arc->center));
	if (arc->start->coo.x == arc->end->coo.x && arc->start->coo.y == arc->end->coo.y)
	{
		if (check.t < check.inter->dist || check.inter->dist == -1) {
			check.inter->coo = hit;
			angle = acosf(vec_dot(v_start, v_hit)) / (2.0f * PI);
			if (vec_cross(v_start, v_hit) < 0)
				angle = 1.0f - angle;
			check.inter->dist = angle;
		}
		return ;
	}
	if (vec_cross(v_start, v_end) >= 0)
		on_arc = (vec_cross(v_start, v_hit) >= -FLT_EPSILON
			&& vec_cross(v_hit, v_end) >= -FLT_EPSILON);
	else
		on_arc = (vec_cross(v_start, v_hit) >= -FLT_EPSILON
			|| vec_cross(v_hit, v_end) >= -FLT_EPSILON);
	if (on_arc && (check.t < check.inter->dist || check.inter->dist == -1))
	{
		check.inter->coo = hit;
		angle = acosf(fminf(fmaxf(vec_dot(v_start, v_hit), -1.0f), 1.0f));
		if (vec_cross(v_start, v_hit) < 0)
			angle = (2.0f * PI) - angle;
		total_angle = acosf(vec_dot(v_start, v_end));
		if (vec_cross(v_start, v_end) < 0)
			total_angle = (2.0f * PI) - total_angle;
		check.inter->dist = angle / total_angle;
	}
}

void	check_possible_seg_arc(t_link *seg, t_link *arc, t_info_check check)
{
	t_vec	hit;
	t_vec	v_hit;
	t_vec	v_end;
	t_vec	v_start;
	int		on_arc;

	if (check.t < 0.0f || check.t > 1.0f)
		return ;
	hit = vec_add(seg->start->coo, vec_scale(check.dir, check.t));
	if (arc->start->coo.x == arc->end->coo.x && arc->start->coo.y == arc->end->coo.y)
	{
		if (check.t < check.inter->dist || check.inter->dist == -1) {
			check.inter->coo = hit;
			check.inter->dist = check.t;
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
	if (on_arc && (check.t < check.inter->dist || check.inter->dist == -1))
	{
		check.inter->coo = hit;
		check.inter->dist = check.t;
	}
}

void	intersect_seg_arc(t_link *seg, t_link *arc, t_inter *inter, void (*f)(t_link *, t_link *, t_info_check))
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
	f(seg, arc, (t_info_check){inter, dir, (-b - sqrt_d) / (2.0f * a)});
	f(seg, arc, (t_info_check){inter, dir, (-b + sqrt_d) / (2.0f * a)});
}

void	intersect_seg_seg(t_link *l1, t_link *l2, t_inter *inter)
{
	t_vec	s1;
	t_vec	s2;
	float	s;
	float	t;

	s1.x = l1->end->coo.x - l1->start->coo.x;
	s1.y = l1->end->coo.y - l1->start->coo.y;
	s2.x = l2->end->coo.x - l2->start->coo.x;
	s2.y = l2->end->coo.y - l2->start->coo.y;
	s = (-s1.y * (l1->start->coo.x - l2->start->coo.x) + s1.x
		* (l1->start->coo.y - l2->start->coo.y)) / (-s2.x * s1.y + s1.x * s2.y);
	t = ( s2.x * (l1->start->coo.y - l2->start->coo.y) - s2.y
		* (l1->start->coo.x - l2->start->coo.x)) / (-s2.x * s1.y + s1.x * s2.y);
	if (s >= 0 && s <= 1 && t > 0 && t < 1 && (t < inter->dist || inter->dist == -1))
	{
		inter->coo.x = l1->start->coo.x + (t * s1.x);
		inter->coo.y = l1->start->coo.y + (t * s1.y);
		inter->dist = t;
	}
}

void	intersect_switch(t_link *l1, t_link *l2, t_inter *inter)
{
	if (l1->type == 0 && l2->type == 0)
		intersect_seg_seg(l1, l2, inter);
	else if (l1->type == 0)
		intersect_seg_arc(l1, l2, inter, &check_possible_seg_arc);
	else if (l2->type == 0)
		intersect_seg_arc(l2, l1, inter, &check_possible_arg_seg);
	else
		intersect_arc_arc(l1, l2, inter);
}

int	find_closest_inter()
{
	//TO-DO
	// find_node(graph->nodes, inter, &graph->nb_nodes)
}

int	find_node(t_node *nodes, t_vec coo, int *current)
{
	int	i;

	i = -1;
	while (++i < *current)
	{
		if (nodes[i].coo.x == coo.x && nodes[i].coo.y == coo.y)
			return (i);
	}
	nodes[i].coo = coo;
	nodes[i].visited = 0;
	nodes[i].connect[0] = NULL;
	++(*current);
	return (i);
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
	while (dest->connect[i] == to_replace && i < 511)
		++i;
	dest->connect[i] = to_add;
}

void	build_primitive_graph(t_list *wpath, t_graph *graph)
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
		found_start = find_node(graph->nodes, wall->start, &graph->nb_nodes);
		found_end = find_node(graph->nodes, wall->start, &graph->nb_nodes);
		graph->links[++j].start = &graph->nodes[found_start];
		graph->links[j].end = &graph->nodes[found_end];
		graph->links[j].center = wall->center;
		graph->links[j].type = wall->mode;
		add_connection(&graph->nodes[found_start], &graph->links[j]);
		add_connection(&graph->nodes[found_end], &graph->links[j]);
		wpath = wpath->next;
		++graph->nb_links;
	}
	if (wpath)
		;//This means that we got too many lines for that tile.
}


//this code suppose that we don't have duplicate of circles and lines, I need to do that.
void	grow_graph(t_graph *graph)
{
	int		i;
	int		i_link;
	int		i_node;

	//there might be an overflow possible ?
	i = -1;
	while (++i < graph->nb_links)
	{
		i_node = find_closest_inter();
		if (i_node != -1)
		{
			graph->links[graph->nb_links].start = &graph->nodes[i_node];
			graph->links[graph->nb_links].end = graph->links[i].end;
			graph->links[graph->nb_links].center = graph->links[i].center;
			graph->links[graph->nb_links].type = graph->links[i].type;
			graph->links[i].end = &graph->nodes[i_node];
			replace_connection(&graph->links[graph->nb_links].end,
				&graph->links[i], &graph->links[graph->nb_links]);
			add_connection(&graph->nodes[i_node], &graph->links[i]);
			add_connection(&graph->nodes[i_node], &graph->links[graph->nb_links]);
			++graph->nb_links;
		}
	}
}

int	build_polygon(t_tile *tile, char id)
{
	char	letter[2];

	letter[0] = id;	
	letter[1] = 0;	
	if (ft_lstsize(tile->wpath) >= 512)
		return (ft_perror(-1, ft_strsjoin((char *[]){"Too many lines for tile \
", letter, ". Expected at most 45 lines.", NULL}), 0), 1);

	
}

int	build_polygons(t_map *map)
{
	t_tile	**tiles;
	int		i;

	tiles = get_tile_dict();
	i = -1;
	while (++i < 256)
	{
		if (!tiles[i])
			continue ;
		if (build_polygon(tiles[i], i))
			return (1);
	}
	return (0);
}
