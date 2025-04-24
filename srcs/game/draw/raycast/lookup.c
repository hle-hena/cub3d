/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lookup.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:53:26 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/24 11:31:27 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_vec	**get_cast_table(void)
{
	static t_vec	*table = NULL;

	return (&table);
}

void	fill_cast_table(t_data *data, int *err)
{
	t_vec	**table;
	int		y;
	int		x;

	table = get_cast_table();
	*table = malloc((data->win_len * data->win_wid) * sizeof(t_vec *));
	if (!*table)
		return (ft_perror(-1, "Internal error: malloc.", 0), *err = 1, VOID);
	y = -1;
	while (++y < data->win_len)
	{
		x = -1;
		while (++x < data->win_wid)
		{
			(*table)[y * data->win_wid + x].x = (float)x / data->win_wid;
			if (y > data->win_len / 2)
				(*table)[y * data->win_wid + x].y = (float)data->win_len * 2
						/ (2.0f * (y - data->win_len / 2));
			else
				(*table)[y * data->win_wid + x].y = (float)data->win_len * 2
						/ (2.0f * (data->win_len / 2 - y));
		}
	}
}
