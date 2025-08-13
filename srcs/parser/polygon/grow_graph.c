/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grow_graph.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:25:41 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/13 17:16:42 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
		// intersect_switch(&graph->links[current], &graph->links[i], &inter, graph);//might not need to pass it as a pointer ?
		intersect_links(&graph->links[current], &graph->links[i], &inter, graph);
	}
	if (inter.dist == -1)
		return (-1);
	return (find_node(graph->nodes, inter.coo, &graph->nb_nodes, 1));
}

int	grow_graph(t_graph *graph)
{
	int		i;
	int		i_node;

	i = -1;
//this code suppose that we don't have duplicate of circles and lines, I need to do that.
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
