/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   primitive_graph.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:21:40 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/28 10:11:10 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	create_new_edge(t_graph *graph, t_wpath *wall, int j)
{
	int	found_start;
	int	found_end;

	found_start = find_node(graph->nodes, wall->start, &graph->nb_nodes, 1);
	found_end = find_node(graph->nodes, wall->end, &graph->nb_nodes, 1);
	graph->links[j] = (t_link){.type = wall->mode, .center = wall->center,
		.start = &graph->nodes[found_start], .visited[0] = 0, .visited[1] = 0,
		.end = &graph->nodes[found_end], .reflectance = wall->reflectance};
	if (add_connection(&graph->nodes[found_start], &graph->links[j]))
		return (1);
	if (found_end != found_start)
	{
		if (add_connection(&graph->nodes[found_end], &graph->links[j]))
			return (1);
	}
	++graph->nb_links;
	return (0);
}

int	build_primitive_graph(t_list *wpath, t_graph *graph)
{
	int		j;

	j = -1;
	graph->nb_links = 0;
	graph->nb_nodes = 0;
	while (wpath && j < 511)
	{
		if (create_new_edge(graph, (t_wpath *)wpath->content, ++j))
			return (1);
		wpath = wpath->next;
	}
	if (wpath)
		return (ft_perror(-1, "Please, define less than 32 paths.", 0), 1);
	return (0);
}
