/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grow_graph.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:25:41 by hle-hena          #+#    #+#             */
/*   Updated: 2025/09/11 14:50:34 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

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
		intersect_links(&graph->links[current], &graph->links[i], &inter,
			graph);
	}
	if (inter.dist == -1)
		return (-1);
	return (find_node(graph->nodes, inter.coo, &graph->nb_nodes, 1));
}

int	grow_graph(t_graph *g, int i_node)
{
	int		i;

	i = -1;
	while (++i < g->nb_links && i < 511)
	{
		i_node = find_closest_inter(g, i);
		if (i_node != -1)
		{
			g->links[g->nb_links] = (t_link){.visited[0] = 0, .visited[1] = 0,
				.reflectance = g->links[i].reflectance, .end = g->links[i].end,
				.start = &g->nodes[i_node], .type = g->links[i].type,
				.center = g->links[i].center};
			g->links[i].end = &g->nodes[i_node];
			if (g->links[i].start != g->links[g->nb_links].end)
				replace_connection(g->links[g->nb_links].end,
					&g->links[i], &g->links[g->nb_links]);
			else
				add_connection(g->links[i].start, &g->links[g->nb_links]);
			add_connection(&g->nodes[i_node], &g->links[i]);
			add_connection(&g->nodes[i_node], &g->links[g->nb_links++]);
		}
	}
	if (i == 512)
		return (ft_perror(-1, "There has been an overflow of the paths", 0), 1);
	return (0);
}
