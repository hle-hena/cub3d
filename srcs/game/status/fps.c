/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fps.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hle-hena <hle-hena@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:04:42 by hle-hena          #+#    #+#             */
/*   Updated: 2025/06/25 13:31:12 by hle-hena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static inline t_uint64	get_time_us(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (t_uint64)tv.tv_sec * 1000000 + tv.tv_usec;
}

void	fps_counter(t_data *data)
{
	static t_uint64	last_frame_time = 0;
	static t_uint64	last_fps_print_time = 0;
	static t_uint64	total_time = 0;
	static t_uint64	frame_count = 0;
	static int		last_option = -1;

	t_uint64	now;
	t_int64		elapsed;
	t_int64		elapsed_since_last_print;

	// Reset stats if option has changed
	if (data->option != last_option)
	{
		last_option = data->option;
		last_frame_time = 0;
		last_fps_print_time = 0;
		total_time = 0;
		frame_count = 0;
		printf("Switched method. Currently doing the %s method\n", data->option ? "new" : "old");
		return;
	}

	now = get_time_us();
	if (last_frame_time == 0)
	{
		last_frame_time = now;
		last_fps_print_time = now;
	}

	elapsed = now - last_frame_time;
	data->delta_t = (float)elapsed / FRAME_TIME_US;
	last_frame_time = now;

	total_time += elapsed;
	frame_count++;

	elapsed_since_last_print = now - last_fps_print_time;
	if (elapsed_since_last_print >= 400000)
	{
		float current_fps = 1000000.0f / elapsed;
		float average_fps = (total_time > 0) ? (frame_count * 1000000.0f / total_time) : 0.0f;

		printf("\rFPS : %.2f | Avg : %.2f", current_fps, average_fps);
		fflush(stdout);

		last_fps_print_time = now;
	}
}
