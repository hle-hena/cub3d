/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_face.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 14:07:45 by hle-hena          #+#    #+#             */
/*   Updated: 2025/09/11 14:50:33 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

int	find_link(t_graph *graph, t_link *link)
{
	int	i;

	i = -1;
	while (++i < graph->nb_links)
	{
		if (graph->links[i].type == 0 && graph->links[i].start == link->start
			&& graph->links[i].end == link->end && link->type == 0)
			return (i);
		else if (link->type == 1 && graph->links[i].start == link->start
			&& graph->links[i].end == link->end && graph->links[i].type == 1
			&& fabs(graph->links[i].center.x - link->center.x) < 1e-6
			&& fabs(graph->links[i].center.y - link->center.y) < 1e-6)
			return (i);
	}
	return (0);
}

void	copy_start_to_end(t_graph *to, t_link *src, int found_start,
	int found_end)
{
	src->visited[0] = 1;
	if (src->start == src->end)
		src->visited[1] = 1;
	to->links[to->nb_links].start = &to->nodes[found_start];
	to->links[to->nb_links].end = &to->nodes[found_end];
}

void	copy_end_to_start(t_graph *to, t_link *src, int found_start,
	int found_end)
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
		copy_start_to_end(to, src, found_start, found_end);
	else
		copy_end_to_start(to, src, found_start, found_end);
	to->nb_links++;
}

void	get_face(t_graph *subgraph, t_link *current_link)
{
	t_node	*current_node;
	t_node	*start_node;
	int		i;

	start_node = current_link->start;
	current_node = current_link->end;
	if (current_link->visited[0])
		current_node = current_link->start;
	if (current_link->visited[0])
		start_node = current_link->end;
	copy_link(subgraph, current_link, start_node);
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
