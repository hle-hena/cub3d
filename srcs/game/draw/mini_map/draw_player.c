/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 13:28:24 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/06 11:55:11 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_player_tip(t_data *data, t_point center, float theta)
{
	t_point	tip;
	t_point	base;

	tip.x = center.x + (data->map->mini_map_scale / 4 + 9) * cos(theta);
	tip.y = center.y + (data->map->mini_map_scale / 4 + 9) * sin(theta);
	base.x = center.x + data->map->mini_map_scale / 4
		* cos(theta + PI / 2 / 2);
	base.y = center.y + data->map->mini_map_scale / 4
		* sin(theta + PI / 2 / 2);
	draw_line(data, tip, base, 0x001B2E);
	base.x = center.x + data->map->mini_map_scale / 4
		* cos(theta - PI / 2 / 2);
	base.y = center.y + data->map->mini_map_scale / 4
		* sin(theta - PI / 2 / 2);
	draw_line(data, tip, base, 0x001B2E);
}

void	draw_player(t_data *data, t_point center, float theta)
{
	t_point	draw;
	int		i;
	float	a;
	float	a_step;
	float	da;

	draw_player_tip(data, center, theta);
	a_step = 2 * PI / 100;
	i = -1;
	while (++i < 100)
	{
		a = i * a_step;
		da = fmod(a - (theta - PI / 2 / 2) + 2 * PI, 2 * PI);
		if (da < PI / 2)
			continue ;
		draw.x = center.x + (int)(data->map->mini_map_scale / 4 * cos(a));
		draw.y = center.y + (int)(data->map->mini_map_scale / 4 * sin(a));
		ft_put_pixel(data, draw, 0x001B2E);
	}
}
