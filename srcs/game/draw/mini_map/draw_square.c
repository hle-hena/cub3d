/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_square.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 22:15:19 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/05 17:06:49 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline void	rotate_point(t_point *draw, t_vec curr, t_point player,
	t_trig vals)
{
	t_vec	centered;

	centered.x = curr.x - player.x;
	centered.y = curr.y - player.y;
	draw->x = centered.x * vals.cos - centered.y * vals.sin;
	draw->y = centered.x * vals.sin + centered.y * vals.cos;
}

int	tile_is_visible(t_data *data, t_point player, t_trig vals, t_point start)
{
	t_point	corners[4];
	t_vec	tmp;
	int		*dx;
	int		*dy;
	int		i;

	dx = (int [4]){0, data->map->mini_map_scale, data->map->mini_map_scale, 0};
	dy = (int [4]){0, 0, data->map->mini_map_scale, data->map->mini_map_scale};
	i = -1;
	while (++i < 4)
	{
		tmp.x = start.x + dx[i];
		tmp.y = start.y + dy[i];
		rotate_point(&corners[i], tmp, player, vals);
		corners[i].x += data->map->mini_map.x;
		corners[i].y += data->map->mini_map.y;
		if (point_is_in_mini_map(data, corners[i]))
			return (1);
	}
	return (0);
}

void	draw_tile(t_data *data, t_point start, t_trig vals, int color)
{
	t_vec	curr;
	t_point	draw;
	t_point	player;

	player.x = data->map->player.x * data->map->mini_map_scale;
	player.y = data->map->player.y * data->map->mini_map_scale;
	if (!tile_is_visible(data, player, vals, start))
		return ;
	curr.y = start.y;
	while (curr.y != start.y + data->map->mini_map_scale - 2)
	{
		curr.x = start.x;
		while (curr.x != start.x + data->map->mini_map_scale - 2)
		{
			rotate_point(&draw, curr, player, vals);
			draw.x += data->map->mini_map.x;
			draw.y += data->map->mini_map.y;
			if (point_is_in_mini_map(data, draw))
				ft_put_pixel(data, draw, color);
			curr.x += 0.5f;
		}
		curr.y += 0.5f;
	}
}
