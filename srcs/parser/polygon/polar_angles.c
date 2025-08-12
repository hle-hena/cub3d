/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polar_angles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 11:28:31 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/12 11:35:59 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	generate_angles(t_node *current, float *angles)
{
	int		j;
	t_vec	out;

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
			out = (t_vec){current->connect[j]->center.y - current->coo.y,
				current->coo.x - current->connect[j]->center.x};
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
