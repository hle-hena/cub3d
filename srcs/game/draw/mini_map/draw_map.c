/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 22:36:25 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/29 16:28:27 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	point_is_in_mini_map(t_data *data, t_point point)
{
	int	dx;
	int	dy;
	int	radius;

	if (point.x < 0 || point.y < 0 || point.x >= data->win_wid
		|| point.y >= data->win_len)
		return (0);
	dx = point.x - data->map->mini_map.x;
	dy = point.y - data->map->mini_map.y;
	radius = data->win_len / 8;
	return (dx * dx + dy * dy < radius * radius);
}

void	draw_tiles(t_data *data)
{
	t_point	curr;
	t_point	start;
	t_trig	vals;

	vals.cos = cos(-(data->map->player.rot + 90) * PI / 180);
	vals.sin = sin(-(data->map->player.rot + 90) * PI / 180);
	curr.y = -1;
	while (++curr.y < data->map->len)
	{
		curr.x = -1;
		while (++curr.x < data->map->wid)
		{
			start.x = (curr.x) * data->map->mini_map_scale + curr.x;
			start.y = (curr.y) * data->map->mini_map_scale + curr.y;
			if (*(data->map->matrix + curr.y * data->map->wid + curr.x)
				== data->map->void_char)
				continue ;
			if (get_tile_dict()[*(data->map->matrix + curr.y * data->map->wid
					+ curr.x)]->is_wall)
				draw_tile(data, start, vals, 0x001B2E);
			else
				draw_tile(data, start, vals, 0xADB6C4);
		}
	}
}

void	draw_mini_map(t_data *data)
{
	draw_tiles(data);
	draw_player(data, data->map->mini_map, -PI / 2);
	draw_circle(data, data->map->mini_map, data->win_len / 8, 0xADB6C4);
}
