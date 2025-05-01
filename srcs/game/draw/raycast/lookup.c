/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lookup.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:53:26 by hle-hena          #+#    #+#             */
/*   Updated: 2025/05/01 12:46:47 by hle-hena         ###   ########.fr       */
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
	*table = malloc((data->render_h * data->render_w) * sizeof(t_vec *));
	if (!*table)
		return (ft_perror(-1, "Internal error: malloc.", 0), *err = 1, VOID);
	y = -1;
	while (++y < data->render_h)
	{
		x = -1;
		while (++x < data->render_w)
		{
			(*table)[y * data->render_w + x].x = (float)x / data->render_w;
			if (y > data->render_h / 2)
				(*table)[y * data->render_w + x].y = (float)data->render_h * 2
						/ (2.0f * (y - data->render_h / 2));
			else
				(*table)[y * data->render_w + x].y = (float)data->render_h * 2
						/ (2.0f * (data->render_h / 2 - y));
		}
	}
}
