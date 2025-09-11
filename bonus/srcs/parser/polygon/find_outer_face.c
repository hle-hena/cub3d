/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_outer_face.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:27:30 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/28 10:11:10 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	find_unvisited_edge(t_graph *graph, int *index)
{
	int	i;

	i = -1;
	while (++i < graph->nb_links)
	{
		if (graph->links[i].visited[0] == 0
			|| graph->links[i].visited[1] == 0)
		{
			*index = i;
			return (0);
		}
	}
	return (1);
}

t_graph	*add_face(t_graph *out, t_graph *new, float *perimeter,
	float found)
{
	int	i;
	int	found_start;
	int	found_end;

	if (!out)
	{
		*perimeter = 0;
		out = ft_calloc(1, sizeof(t_graph));
	}
	if (!out)
		return (ft_perror(-1, "Internal error: malloc.", 0), NULL);
	i = -1;
	*perimeter += found;
	while (++i < new->nb_links)
	{
		found_start = find_node(out->nodes, new->links[i].start->coo,
				&out->nb_nodes, 1);
		found_end = find_node(out->nodes, new->links[i].end->coo,
				&out->nb_nodes, 1);
		out->links[out->nb_links] = new->links[i];
		out->links[out->nb_links].start = &out->nodes[found_start];
		out->links[out->nb_links].end = &out->nodes[found_end];
		++out->nb_links;
	}
	return (out);
}

t_graph	*retrieve_outer_face(t_graph *graph, float *perimeter)
{
	int		index;
	t_graph	*temp_graph;
	t_graph	*outer_face;
	float	temp;

	temp_graph = malloc(sizeof(t_graph));
	outer_face = NULL;
	if (!temp_graph)
		return (ft_perror(-1, "Internal error: malloc.", 0), NULL);
	sort_edges(graph);
	while (1)
	{
		if (find_unvisited_edge(graph, &index))
			break ;
		temp_graph->nb_links = 0;
		temp_graph->nb_nodes = 0;
		get_face(temp_graph, &graph->links[index]);
		if (polygon_area(temp_graph, &temp) >= 0)
			outer_face = add_face(outer_face, temp_graph, perimeter, temp);
	}
	ft_del((void **)&temp_graph);
	return (outer_face);
}
