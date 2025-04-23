/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lookup.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:53:26 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/23 15:21:39 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_vec	***get_cast_table(void)
{
	static t_vec	**table = NULL;

	return (&table);
}

void	del_previous(t_vec ***table, int y_max)
{
	int	y;

	y = -1;
	while (++y < y_max)
		ft_del((void **)&(*table)[y]);
	ft_del((void **)&(*table));
	*table = NULL;
}

void	fill_cast_table(t_data *data, int *err)
{
	t_vec	***table;
	int		y;
	int		x;

	table = get_cast_table();
	*table = malloc(data->win_len * sizeof(t_vec *));
	if (!*table)
		return (ft_perror(-1, "Internal error: malloc.", 0), *err = 1, VOID);
	y = -1;
	while (++y < data->win_len)
	{
		(*table)[y] = malloc(data->win_wid * sizeof(t_vec));
		if (!(*table)[y])
			return (ft_perror(-1, "Internal error: malloc.", 0), *err = 1,
				del_previous(table, y), VOID);
		x = -1;
		while (++x < data->win_wid)
		{
			(*table)[y][x].x = (float)x / data->win_wid;
			if (y > data->win_len / 2)
				(*table)[y][x].y = (float)data->win_len * 2 / (2.0f
						* (y - data->win_len / 2));
			else
				(*table)[y][x].y = (float)data->win_len * 2 / (2.0f
						* (data->win_len / 2 - y));
		}
	}
}
