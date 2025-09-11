/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 21:49:44 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/29 15:17:57 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	do_colisions(t_data *data, t_vec delta, float len)
{
	int	wall_x;
	int	wall_y;

	delta.x = (delta.x / len) * MOVE_SPEED;
	delta.y = (delta.y / len) * MOVE_SPEED;
	wall_x = (int)(data->map->player.x + delta.x);
	wall_y = (int)(data->map->player.y + delta.y);
	if (!get_tile_dict()[*(data->map->matrix + (int)data->map->player.y
			* data->map->wid + wall_x)]->is_wall)
		data->map->player.x += delta.x;
	else
	{
		data->map->player.x = wall_x + 1.01f;
		if (delta.x > 0)
			data->map->player.x = wall_x - 0.01f;
	}
	if (!get_tile_dict()[*(data->map->matrix + (int)wall_y * data->map->wid
			+ (int)data->map->player.x)]->is_wall)
		data->map->player.y += delta.y;
	else
	{
		data->map->player.y = wall_y + 1.01f;
		if (delta.y > 0)
			data->map->player.y = wall_y - 0.01f;
	}
}

void	move_event(t_data *data)
{
	float	cos_val;
	float	sin_val;
	t_vec	delta;
	float	len;

	cos_val = cos(data->map->player.rot * PI / 180);
	sin_val = sin(data->map->player.rot * PI / 180);
	data->event.rot -= (data->event.rot_left - data->event.rot_right) * 20;
	if (data->event.rot < 0)
		data->map->player.rot += (float)data->event.rot * ROT_SPEED + 360;
	else
		data->map->player.rot += ceil((float)data->event.rot * ROT_SPEED) + 360;
	data->map->player.rot = fmodf(data->map->player.rot, 360);
	data->event.rot = 0;
	delta.x = cos_val * (data->event.mo_f + data->event.mo_b)
		- sin_val * (data->event.mo_l + data->event.mo_r);
	delta.y = sin_val * (data->event.mo_f + data->event.mo_b)
		+ cos_val * (data->event.mo_l + data->event.mo_r);
	len = sqrt(delta.x * delta.x + delta.y * delta.y);
	if (len > 0.001f)
		do_colisions(data, delta, len);
}

int	event_loop(t_data *data)
{
	fps_counter(data);
	move_event(data);
	cast_rays(data);
	draw_mini_map(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img.img, 0, 0);
	if (data->event.echap)
		mlx_close(data);
	return (0);
}
