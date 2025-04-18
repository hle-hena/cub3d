/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 21:49:44 by hle-hena          #+#    #+#             */
/*   Updated: 2025/04/19 18:59:06 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	bzero_img(t_data *data)
{
	ft_bzero(data->img.data, data->win_len * data->win_wid * sizeof(int));
}

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
		data->map->player.rot += ceil((float)data->event.rot * ROT_SPEED) + 360;
	data->map->player.rot = fmodf(data->map->player.rot, 360);
	data->event.rot = 0;
	delta.x = cos_val * (data->event.mo_f + data->event.mo_b)
		- sin_val * (data->event.mo_l + data->event.mo_r);
	delta.y = sin_val * (data->event.mo_f + data->event.mo_b)
		+ cos_val * (data->event.mo_l + data->event.mo_r);
	len = sqrt(delta.x * delta.x + delta.y * delta.y);
	if (len > 0.001f)
	{
		delta.x = (delta.x / len) * MOVE_SPEED;
		delta.y = (delta.y / len) * MOVE_SPEED;
		if (data->map->matrix[(int)(data->map->player.y)][(int)(data->map->player.x + delta.x)] != '1')
			data->map->player.x += delta.x;
		if (data->map->matrix[(int)(data->map->player.y + delta.y)][(int)(data->map->player.x)] != '1')
			data->map->player.y += delta.y;
	}
}

int	event_loop(t_data *data)
{
	    static struct timeval last_time = {0, 0};
    struct timeval current_time;
    double elapsed_time;
    static int frame_count = 0;
    static double fps = 0;

    // Get current time
    gettimeofday(&current_time, NULL);
    
    // Calculate elapsed time in seconds
    elapsed_time = (current_time.tv_sec - last_time.tv_sec) + 
                   (current_time.tv_usec - last_time.tv_usec) / 1000000.0;

    // Update FPS every second
    frame_count++;
    if (elapsed_time >= 1.0) {
        fps = frame_count / elapsed_time;
        printf("\rFPS: %.2f", fps);  // Print FPS to the terminal, overwrite previous value
        fflush(stdout);  // Ensure it updates immediately
        frame_count = 0;
        last_time = current_time;
    }
	
	bzero_img(data);
	move_event(data);
	cast_rays(data);
	// draw_mini_map(data);
	// mlx_put_image_to_window(data->mlx, data->win, data->img.img, 0, 0);
	if (data->event.echap)
		mlx_close(data);
	return (0);
}
