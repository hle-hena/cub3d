/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graph_connection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 10:23:41 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/12 11:18:53 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	add_connection(t_node *dest, t_link *to_add)
{
	int	i;

	i = 0;
	while (dest->connect[i] && i < 511)
		++i;
	if (i == 512)
	{
		ft_perror(-1, "There has been an overflow of the connections.", 0);
		return (1);
	}
	dest->connect[i] = to_add;
	dest->connect[i + 1] = NULL;
	return (0);
}

void	replace_connection(t_node *dest, t_link *to_replace, t_link *to_add)
{
	int	i;

	i = 0;
	while (dest->connect[i] != to_replace && i < 511)
		++i;
	dest->connect[i] = to_add;
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
