/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 21:49:44 by hle-hena          #+#    #+#             */
/*   Updated: 2025/06/17 15:11:28 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	move_event(t_data *data)
{
	float	cos_val;
	float	sin_val;
	t_vec	delta;
	float	len;

	cos_val = cos(data->map->player.rot * PI / 180);
	sin_val = sin(data->map->player.rot * PI / 180);
	if (data->event.rot < 0)
		data->map->player.rot += (float)data->event.rot * ROT_SPEED + 360;
	else
		data->map->player.rot -= (float)data->event.rot * -ROT_SPEED + 360;
	data->map->player.rot = fmodf(data->map->player.rot, 360);
	data->event.rot = 0;
	delta.x = cos_val * (data->event.mo_f + data->event.mo_b)
		- sin_val * (data->event.mo_l + data->event.mo_r);
	delta.y = sin_val * (data->event.mo_f + data->event.mo_b)
		+ cos_val * (data->event.mo_l + data->event.mo_r);
	len = sqrt(delta.x * delta.x + delta.y * delta.y);
	if (len > 0.001f)
	{
		delta.x = (delta.x / len) * MOVE_SPEED * data->delta_t;
		delta.y = (delta.y / len) * MOVE_SPEED * data->delta_t;
		//no-clip
		int	no_clip = 1;
		if (no_clip)
		{
			data->map->player.x += delta.x;
			data->map->player.y += delta.y;
		}
		else
		{
			int wall_x = (int)(data->map->player.x + delta.x);
			int wall_y = (int)(data->map->player.y + delta.y);
			if (!get_tile_dict()[*(data->map->matrix + (int)data->map->player.y * data->map->wid + wall_x)]->is_wall)
				data->map->player.x += delta.x;
			else
			{
				if (delta.x > 0)
					data->map->player.x = wall_x - 0.01f;
				else if (delta.x < 0)
					data->map->player.x = wall_x + 1.01f;
			}
			if (!get_tile_dict()[*(data->map->matrix + (int)wall_y * data->map->wid + (int)data->map->player.x)]->is_wall)
				data->map->player.y += delta.y;
			else
			{
				if (delta.y > 0)
					data->map->player.y = wall_y - 0.01f;
				else if (delta.y < 0)
					data->map->player.y = wall_y + 1.01f;
			}
		}
	}
}

int	event_loop(t_data *data)
{
	fps_counter(data);
	move_event(data);
	cast_rays(data);
	// draw_mini_map(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img[data->img_buffer].img, 0, 0);
	data->img_buffer++;
	if (data->img_buffer >= IMG_BUFFER)
		data->img_buffer = 0;
	if (data->event.echap)
		mlx_close(data);
	return (0);
}
