/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fps.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:04:42 by hle-hena          #+#    #+#             */
/*   Updated: 2025/08/18 13:37:50 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_uint64	get_time_us(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((t_uint64)tv.tv_sec * 1000000 + tv.tv_usec);
}

void	fps_counter(t_data *data, t_uint64 now)
{
	static t_uint64	last_frame_time = 0;
	static t_uint64	last_fps_print_time = 0;
	static t_uint64	total_time = 0;
	static t_uint64	frame_count = 0;
	t_int64			elapsed;

	now = get_time_us();
	if (last_frame_time == 0)
	{
		last_frame_time = now;
		last_fps_print_time = now;
	}
	elapsed = now - last_frame_time;
	data->delta_t = (float)elapsed / (1000000 / 60);
	last_frame_time = now;
	total_time += elapsed;
	frame_count++;
	if (now - last_fps_print_time >= 400000)
	{
		printf("\rFPS : %.2f | Avg : %.2f", 1000000.0f / elapsed,
			frame_count * 1000000.0f / ft_max(total_time, 1));
		fflush(stdout);
		last_fps_print_time = now;
	}
}
