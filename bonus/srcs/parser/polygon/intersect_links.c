/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_links.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:29:09 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/28 10:11:10 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	intersect_links(t_link *l1, t_link *l2, t_inter *inter, t_graph *graph)
{
	t_inter	temp;

	temp.dist = -1;
	temp.pos = -1;
	if (l1->type == 0 && l2->type == 0)
		temp = intersect_slseg(l1, l2, graph);
	else if (l1->type == 0)
		temp = intersect_alsl(l1, l2, graph, &check_solutions_slarc);
	else if (l2->type == 0)
	{
		temp = intersect_alsl(l2, l1, graph, &check_solutions_alseg);
		temp.dist = temp.pos;
	}
	else
		temp = intersect_alarc(l1, l2, graph);
	if (temp.dist > -0.5f && (temp.dist < inter->dist || inter->dist < 0))
		*inter = (t_inter){temp.coo, (t_vec){0}, 0, temp.dist};
}
