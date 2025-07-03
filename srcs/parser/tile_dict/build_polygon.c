/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_polygon.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 10:55:56 by hle-hena          #+#    #+#             */
/*   Updated: 2025/07/03 15:58:34 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_vec	intersect_link(t_link *l1, t_link *l2, t_vec *prev_found_inter,
	float *prev_dist)
{
	t_vec	s1;
	t_vec	s2;
	float	s;
	float	t;

	//this is only if link->type == 0, so if both links are straight lines.
	//Is it possible to make it work with both link->type == 0 and == 1, in the same function ?
	//Or do I need 4 function, one for each combination of the two types ?
	//So one for intersection between segments, one for intersection between arc, one for segment to arc and one for arc to segment
	s1.x = l1->end->coo.x - l1->start->coo.x;
	s1.y = l1->end->coo.y - l1->start->coo.y;
	s2.x = l2->end->coo.x - l2->start->coo.x;
	s2.y = l2->end->coo.y - l2->start->coo.y;
	s = (-s1.y * (l1->start->coo.x - l2->start->coo.x) + s1.x
		* (l1->start->coo.y - l2->start->coo.y)) / (-s2.x * s1.y + s1.x * s2.y);
	t = ( s2.x * (l1->start->coo.y - l2->start->coo.y) - s2.y
		* (l1->start->coo.x - l2->start->coo.x)) / (-s2.x * s1.y + s1.x * s2.y);
	if (s >= 0 && s <= 1 && t >= 0 && t <= 1 && t < *prev_dist)
	{
		prev_found_inter->x = l1->start->coo.x + (t * s1.x);
		prev_found_inter->y = l1->start->coo.y + (t * s1.y);
		*prev_dist = t;
		return (*prev_found_inter);
	}
	return ((t_vec){-1, -1});
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

void	grow_graph(t_graph *graph)
{
	int		i;
	int		i_link;
	int		i_node;


	//need to walk though this func like a dumb robot, to see if there are any cases I missed.
	//also dont know if I need to set visited ????
	//there might be an overflow possible ?
	i = -1;
	while (++i < graph->nb_links)
	{
		i_node = find_closest_inter();
		//if no inter means end of line, this if statement is useless.
		//I think no inter should output smth like i_node = -1.
		//-> this means that the find_closest should  output the index to the found inter
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
